#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
/*
 * TLSConfig.actor.cpp
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define PRIVATE_EXCEPT_FOR_TLSCONFIG_CPP
#include "flow/TLSConfig.actor.h"
#undef PRIVATE_EXCEPT_FOR_TLSCONFIG_CPP

// To force typeinfo to only be emitted once.
TLSPolicy::~TLSPolicy() {}

#ifdef TLS_DISABLED

void LoadedTLSConfig::print(FILE* fp) {
	fprintf(fp, "Cannot print LoadedTLSConfig.  TLS support is not enabled.\n");
}

#else // TLS is enabled

#include <algorithm>
#include <cstring>
#include <exception>
#include <map>
#include <set>
#include <openssl/objects.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <utility>
#include <boost/asio/ssl/context.hpp>

// This include breaks module dependencies, but we need to do async file reads.
// So either we include fdbrpc here, or this file is moved to fdbrpc/, and then
// Net2, which depends on us, includes fdbrpc/.
//
// Either way, the only way to break this dependency cycle is to move all of
// AsyncFile to flow/
#include "fdbrpc/IAsyncFile.h"
#include "flow/Platform.h"

#include "flow/FastRef.h"
#include "flow/Trace.h"
#include "flow/genericactors.actor.h"
#include "flow/actorcompiler.h" // This must be the last #include.

std::vector<std::string> LoadedTLSConfig::getVerifyPeers() const {
	if (tlsVerifyPeers.size()) {
		return tlsVerifyPeers;
	}

	std::string envVerifyPeers;
	if (platform::getEnvironmentVar("FDB_TLS_VERIFY_PEERS", envVerifyPeers)) {
		return { envVerifyPeers };
	}

	return { "Check.Valid=1" };
}

std::string LoadedTLSConfig::getPassword() const {
	if (tlsPassword.size()) {
		return tlsPassword;
	}

	std::string envPassword;
	platform::getEnvironmentVar("FDB_TLS_PASSWORD", envPassword);
	return envPassword;
}

void LoadedTLSConfig::print(FILE* fp) {
	int num_certs = 0;
	boost::asio::ssl::context context(boost::asio::ssl::context::tls);
	try {
		ConfigureSSLContext(*this, &context);
	} catch (Error& e) {
		fprintf(fp, "There was an error in loading the certificate chain.\n");
		throw;
	}

	X509_STORE* store = SSL_CTX_get_cert_store(context.native_handle());
	X509_STORE_CTX* store_ctx = X509_STORE_CTX_new();
	X509* cert = SSL_CTX_get0_certificate(context.native_handle());
	X509_STORE_CTX_init(store_ctx, store, cert, nullptr);

	X509_verify_cert(store_ctx);
	STACK_OF(X509)* chain = X509_STORE_CTX_get0_chain(store_ctx);

	X509_print_fp(fp, cert);

	num_certs = sk_X509_num(chain);
	if (num_certs) {
		for (int i = 0; i < num_certs; i++) {
			printf("\n");
			X509* cert = sk_X509_value(chain, i);
			X509_print_fp(fp, cert);
		}
	}

	X509_STORE_CTX_free(store_ctx);
}

void ConfigureSSLContext(const LoadedTLSConfig& loaded,
                         boost::asio::ssl::context* context,
                         std::function<void()> onPolicyFailure) {
	try {
		context->set_options(boost::asio::ssl::context::default_workarounds);
		context->set_verify_mode(boost::asio::ssl::context::verify_peer |
		                         boost::asio::ssl::verify_fail_if_no_peer_cert);

		if (loaded.isTLSEnabled()) {
			auto tlsPolicy = makeReference<TLSPolicy>(loaded.getEndpointType());
			tlsPolicy->set_verify_peers({ loaded.getVerifyPeers() });

			context->set_verify_callback(
			    [policy = tlsPolicy, onPolicyFailure](bool preverified, boost::asio::ssl::verify_context& ctx) {
				    bool success = policy->verify_peer(preverified, ctx.native_handle());
				    if (!success) {
					    onPolicyFailure();
				    }
				    return success;
			    });
		} else {
			// Insecurely always except if TLS is not enabled.
			context->set_verify_callback([](bool, boost::asio::ssl::verify_context&) { return true; });
		}

		context->set_password_callback([password = loaded.getPassword()](
		                                   size_t, boost::asio::ssl::context::password_purpose) { return password; });

		const std::string& CABytes = loaded.getCABytes();
		if (CABytes.size()) {
			context->add_certificate_authority(boost::asio::buffer(CABytes.data(), CABytes.size()));
		}

		const std::string& keyBytes = loaded.getKeyBytes();
		if (keyBytes.size()) {
			context->use_private_key(boost::asio::buffer(keyBytes.data(), keyBytes.size()),
			                         boost::asio::ssl::context::pem);
		}

		const std::string& certBytes = loaded.getCertificateBytes();
		if (certBytes.size()) {
			context->use_certificate_chain(boost::asio::buffer(certBytes.data(), certBytes.size()));
		}
	} catch (boost::system::system_error& e) {
		TraceEvent("TLSConfigureError")
		    .detail("What", e.what())
		    .detail("Value", e.code().value())
		    .detail("WhichMeans", TLSPolicy::ErrorString(e.code()));
		throw tls_error();
	}
}

std::string TLSConfig::getCertificatePathSync() const {
	if (tlsCertPath.size()) {
		return tlsCertPath;
	}

	std::string envCertPath;
	if (platform::getEnvironmentVar("FDB_TLS_CERTIFICATE_FILE", envCertPath)) {
		return envCertPath;
	}

	const char* defaultCertFileName = "fdb.pem";
	if (fileExists(defaultCertFileName)) {
		return defaultCertFileName;
	}

	if (fileExists(joinPath(platform::getDefaultConfigPath(), defaultCertFileName))) {
		return joinPath(platform::getDefaultConfigPath(), defaultCertFileName);
	}

	return std::string();
}

std::string TLSConfig::getKeyPathSync() const {
	if (tlsKeyPath.size()) {
		return tlsKeyPath;
	}

	std::string envKeyPath;
	if (platform::getEnvironmentVar("FDB_TLS_KEY_FILE", envKeyPath)) {
		return envKeyPath;
	}

	const char* defaultCertFileName = "fdb.pem";
	if (fileExists(defaultCertFileName)) {
		return defaultCertFileName;
	}

	if (fileExists(joinPath(platform::getDefaultConfigPath(), defaultCertFileName))) {
		return joinPath(platform::getDefaultConfigPath(), defaultCertFileName);
	}

	return std::string();
}

std::string TLSConfig::getCAPathSync() const {
	if (tlsCAPath.size()) {
		return tlsCAPath;
	}

	std::string envCAPath;
	platform::getEnvironmentVar("FDB_TLS_CA_FILE", envCAPath);
	return envCAPath;
}

LoadedTLSConfig TLSConfig::loadSync() const {
	LoadedTLSConfig loaded;

	const std::string certPath = getCertificatePathSync();
	if (certPath.size()) {
		try {
			loaded.tlsCertBytes = readFileBytes(certPath, FLOW_KNOBS->CERT_FILE_MAX_SIZE);
		} catch (Error& e) {
			fprintf(stderr, "Error reading TLS Certificate [%s]: %s\n", certPath.c_str(), e.what());
			throw;
		}
	} else {
		loaded.tlsCertBytes = tlsCertBytes;
	}

	const std::string keyPath = getKeyPathSync();
	if (keyPath.size()) {
		try {
			loaded.tlsKeyBytes = readFileBytes(keyPath, FLOW_KNOBS->CERT_FILE_MAX_SIZE);
		} catch (Error& e) {
			fprintf(stderr, "Error reading TLS Key [%s]: %s\n", keyPath.c_str(), e.what());
			throw;
		}
	} else {
		loaded.tlsKeyBytes = tlsKeyBytes;
	}

	const std::string CAPath = getCAPathSync();
	if (CAPath.size()) {
		try {
			loaded.tlsCABytes = readFileBytes(CAPath, FLOW_KNOBS->CERT_FILE_MAX_SIZE);
		} catch (Error& e) {
			fprintf(stderr, "Error reading TLS CA [%s]: %s\n", CAPath.c_str(), e.what());
			throw;
		}
	} else {
		loaded.tlsCABytes = tlsCABytes;
	}

	loaded.tlsPassword = tlsPassword;
	loaded.tlsVerifyPeers = tlsVerifyPeers;
	loaded.endpointType = endpointType;

	return loaded;
}

// And now do the same thing, but async...

															#line 279 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
namespace {
// This generated class is to be used only via readEntireFile()
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
template <class ReadEntireFileActor>
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
class ReadEntireFileActorState {
															#line 286 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
public:
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	ReadEntireFileActorState(std::string const& filename,std::string* const& destination) 
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		 : filename(filename),
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   destination(destination)
															#line 295 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
	{
		fdb_probe_actor_create("readEntireFile", reinterpret_cast<unsigned long>(this));

	}
	~ReadEntireFileActorState() 
	{
		fdb_probe_actor_destroy("readEntireFile", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 278 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			StrictFuture<Reference<IAsyncFile>> __when_expr_0 = IAsyncFileSystem::filesystem()->open(filename, IAsyncFile::OPEN_READONLY | IAsyncFile::OPEN_UNCACHED, 0);
															#line 278 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 312 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 1;
															#line 278 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >*>(static_cast<ReadEntireFileActor*>(this)));
															#line 317 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~ReadEntireFileActorState();
		static_cast<ReadEntireFileActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 280 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		StrictFuture<int64_t> __when_expr_1 = file->size();
															#line 280 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 342 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 2;
															#line 280 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ReadEntireFileActor, 1, int64_t >*>(static_cast<ReadEntireFileActor*>(this)));
															#line 347 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> const& __file,int loopDepth) 
	{
															#line 278 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		file = __file;
															#line 356 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Reference<IAsyncFile> && __file,int loopDepth) 
	{
		file = std::move(__file);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state > 0) static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 0;
		static_cast<ReadEntireFileActor*>(this)->ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >::remove();

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> const& value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >*,Reference<IAsyncFile> && value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >*,Error err) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 281 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (filesize > FLOW_KNOBS->CERT_FILE_MAX_SIZE)
															#line 423 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		{
															#line 282 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			return a_body1Catch1(file_too_large(), loopDepth);
															#line 427 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		}
															#line 284 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		destination->resize(filesize);
															#line 285 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		StrictFuture<Void> __when_expr_2 = success(file->read(&((*destination)[0]), filesize, 0));
															#line 285 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 435 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
		static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 3;
															#line 285 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ReadEntireFileActor, 2, Void >*>(static_cast<ReadEntireFileActor*>(this)));
															#line 440 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(int64_t const& __filesize,int loopDepth) 
	{
															#line 280 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		filesize = __filesize;
															#line 449 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(int64_t && __filesize,int loopDepth) 
	{
		filesize = std::move(__filesize);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state > 0) static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 0;
		static_cast<ReadEntireFileActor*>(this)->ActorCallback< ReadEntireFileActor, 1, int64_t >::remove();

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 1, int64_t >*,int64_t const& value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 1, int64_t >*,int64_t && value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ReadEntireFileActor, 1, int64_t >*,Error err) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(Void const& _,int loopDepth) 
	{
															#line 286 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (!static_cast<ReadEntireFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEntireFileActorState(); static_cast<ReadEntireFileActor*>(this)->destroy(); return 0; }
															#line 516 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		new (&static_cast<ReadEntireFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadEntireFileActorState();
		static_cast<ReadEntireFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont3(Void && _,int loopDepth) 
	{
															#line 286 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (!static_cast<ReadEntireFileActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ReadEntireFileActorState(); static_cast<ReadEntireFileActor*>(this)->destroy(); return 0; }
															#line 528 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		new (&static_cast<ReadEntireFileActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ReadEntireFileActorState();
		static_cast<ReadEntireFileActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ReadEntireFileActor*>(this)->actor_wait_state > 0) static_cast<ReadEntireFileActor*>(this)->actor_wait_state = 0;
		static_cast<ReadEntireFileActor*>(this)->ActorCallback< ReadEntireFileActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ReadEntireFileActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ReadEntireFileActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::string filename;
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::string* destination;
															#line 278 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	Reference<IAsyncFile> file;
															#line 280 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	int64_t filesize;
															#line 607 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
};
// This generated class is to be used only via readEntireFile()
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
class ReadEntireFileActor final : public Actor<Void>, public ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >, public ActorCallback< ReadEntireFileActor, 1, int64_t >, public ActorCallback< ReadEntireFileActor, 2, Void >, public FastAllocated<ReadEntireFileActor>, public ReadEntireFileActorState<ReadEntireFileActor> {
															#line 612 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
public:
	using FastAllocated<ReadEntireFileActor>::operator new;
	using FastAllocated<ReadEntireFileActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >;
friend struct ActorCallback< ReadEntireFileActor, 1, int64_t >;
friend struct ActorCallback< ReadEntireFileActor, 2, Void >;
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	ReadEntireFileActor(std::string const& filename,std::string* const& destination) 
															#line 625 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		 : Actor<Void>(),
		   ReadEntireFileActorState<ReadEntireFileActor>(filename, destination)
	{
		fdb_probe_actor_enter("readEntireFile", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("readEntireFile");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("readEntireFile", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ReadEntireFileActor, 0, Reference<IAsyncFile> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ReadEntireFileActor, 1, int64_t >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ReadEntireFileActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
[[nodiscard]] static Future<Void> readEntireFile( std::string const& filename, std::string* const& destination ) {
															#line 277 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	return Future<Void>(new ReadEntireFileActor(filename, destination));
															#line 655 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
}

#line 288 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"

															#line 660 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
// This generated class is to be used only via loadAsync()
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
template <class TLSConfig_LoadAsyncActor>
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
class TLSConfig_LoadAsyncActorState {
															#line 666 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
public:
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	TLSConfig_LoadAsyncActorState(const TLSConfig* const& self) 
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		 : self(self),
															#line 290 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   loaded(),
															#line 291 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   reads(),
															#line 293 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   certIdx(-1),
															#line 294 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   keyIdx(-1),
															#line 295 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   caIdx(-1),
															#line 297 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		   certPath(self->getCertificatePathSync())
															#line 685 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
	{
		fdb_probe_actor_create("loadAsync", reinterpret_cast<unsigned long>(this));

	}
	~TLSConfig_LoadAsyncActorState() 
	{
		fdb_probe_actor_destroy("loadAsync", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 298 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			if (certPath.size())
															#line 700 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			{
															#line 299 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				reads.push_back(readEntireFile(certPath, &loaded.tlsCertBytes));
															#line 300 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				certIdx = reads.size() - 1;
															#line 706 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
			else
			{
															#line 302 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				loaded.tlsCertBytes = self->tlsCertBytes;
															#line 712 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
															#line 305 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			keyPath = self->getKeyPathSync();
															#line 306 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			if (keyPath.size())
															#line 718 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			{
															#line 307 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				reads.push_back(readEntireFile(keyPath, &loaded.tlsKeyBytes));
															#line 308 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				keyIdx = reads.size() - 1;
															#line 724 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
			else
			{
															#line 310 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				loaded.tlsKeyBytes = self->tlsKeyBytes;
															#line 730 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
															#line 313 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			CAPath = self->getCAPathSync();
															#line 314 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			if (CAPath.size())
															#line 736 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			{
															#line 315 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				reads.push_back(readEntireFile(CAPath, &loaded.tlsCABytes));
															#line 316 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				caIdx = reads.size() - 1;
															#line 742 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
			else
			{
															#line 318 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				loaded.tlsCABytes = self->tlsCABytes;
															#line 748 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
			try {
															#line 322 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				StrictFuture<Void> __when_expr_0 = waitForAll(reads);
															#line 322 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				if (static_cast<TLSConfig_LoadAsyncActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 755 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<TLSConfig_LoadAsyncActor*>(this)->actor_wait_state = 1;
															#line 322 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >*>(static_cast<TLSConfig_LoadAsyncActor*>(this)));
															#line 760 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
				loopDepth = 0;
			}
			catch (Error& error) {
				loopDepth = a_body1Catch2(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1Catch2(unknown_error(), loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch1(Error error,int loopDepth=0) 
	{
		this->~TLSConfig_LoadAsyncActorState();
		static_cast<TLSConfig_LoadAsyncActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 337 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		loaded.tlsPassword = self->tlsPassword;
															#line 338 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		loaded.tlsVerifyPeers = self->tlsVerifyPeers;
															#line 339 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		loaded.endpointType = self->endpointType;
															#line 341 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
		if (!static_cast<TLSConfig_LoadAsyncActor*>(this)->SAV<LoadedTLSConfig>::futures) { (void)(loaded); this->~TLSConfig_LoadAsyncActorState(); static_cast<TLSConfig_LoadAsyncActor*>(this)->destroy(); return 0; }
															#line 795 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		new (&static_cast<TLSConfig_LoadAsyncActor*>(this)->SAV< LoadedTLSConfig >::value()) LoadedTLSConfig(std::move(loaded)); // state_var_RVO
		this->~TLSConfig_LoadAsyncActorState();
		static_cast<TLSConfig_LoadAsyncActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 324 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			if (certIdx != -1 && reads[certIdx].isError())
															#line 808 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			{
															#line 325 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				fprintf(stderr, "Failure reading TLS Certificate [%s]: %s\n", certPath.c_str(), e.what());
															#line 812 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
			}
			else
			{
															#line 326 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
				if (keyIdx != -1 && reads[keyIdx].isError())
															#line 818 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
				{
															#line 327 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
					fprintf(stderr, "Failure reading TLS Key [%s]: %s\n", keyPath.c_str(), e.what());
															#line 822 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
				}
				else
				{
															#line 328 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
					if (caIdx != -1 && reads[caIdx].isError())
															#line 828 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
					{
															#line 329 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
						fprintf(stderr, "Failure reading TLS Key [%s]: %s\n", CAPath.c_str(), e.what());
															#line 832 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
					}
					else
					{
															#line 331 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
						fprintf(stderr, "Failure reading TLS needed file: %s\n", e.what());
															#line 838 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
					}
				}
			}
															#line 334 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
			return a_body1Catch1(e, loopDepth);
															#line 844 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont8(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TLSConfig_LoadAsyncActor*>(this)->actor_wait_state > 0) static_cast<TLSConfig_LoadAsyncActor*>(this)->actor_wait_state = 0;
		static_cast<TLSConfig_LoadAsyncActor*>(this)->ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadAsync", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadAsync", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont9(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	const TLSConfig* self;
															#line 290 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	LoadedTLSConfig loaded;
															#line 291 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::vector<Future<Void>> reads;
															#line 293 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	int32_t certIdx;
															#line 294 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	int32_t keyIdx;
															#line 295 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	int32_t caIdx;
															#line 297 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::string certPath;
															#line 305 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::string keyPath;
															#line 313 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	std::string CAPath;
															#line 960 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
};
// This generated class is to be used only via loadAsync()
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
class TLSConfig_LoadAsyncActor final : public Actor<LoadedTLSConfig>, public ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >, public FastAllocated<TLSConfig_LoadAsyncActor>, public TLSConfig_LoadAsyncActorState<TLSConfig_LoadAsyncActor> {
															#line 965 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
public:
	using FastAllocated<TLSConfig_LoadAsyncActor>::operator new;
	using FastAllocated<TLSConfig_LoadAsyncActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<LoadedTLSConfig>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >;
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	TLSConfig_LoadAsyncActor(const TLSConfig* const& self) 
															#line 976 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
		 : Actor<LoadedTLSConfig>(),
		   TLSConfig_LoadAsyncActorState<TLSConfig_LoadAsyncActor>(self)
	{
		fdb_probe_actor_enter("loadAsync", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("loadAsync");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("loadAsync", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TLSConfig_LoadAsyncActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
[[nodiscard]] Future<LoadedTLSConfig> TLSConfig::loadAsync( const TLSConfig* const& self ) {
															#line 289 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"
	return Future<LoadedTLSConfig>(new TLSConfig_LoadAsyncActor(self));
															#line 1003 "/usr/src/libfdb_c/flow/TLSConfig.actor.g.cpp"
}

#line 343 "/usr/src/libfdb_c/flow/TLSConfig.actor.cpp"

std::string TLSPolicy::ErrorString(boost::system::error_code e) {
	char* str = ERR_error_string(e.value(), nullptr);
	return std::string(str);
}

std::string TLSPolicy::toString() const {
	std::stringstream ss;
	ss << "TLSPolicy{ Rules=[";
	for (const auto& r : rules) {
		ss << " " << r.toString() << ",";
	}
	ss << " ] }";
	return ss.str();
}

std::string TLSPolicy::Rule::toString() const {
	std::stringstream ss;

	ss << "Rule{ verify_cert=" << verify_cert << ", verify_time=" << verify_time;
	ss << ", Subject=[";
	for (const auto& s : subject_criteria) {
		ss << " { NID=" << s.first << ", Criteria=" << s.second.criteria << "},";
	}
	ss << " ], Issuer=[";
	for (const auto& s : issuer_criteria) {
		ss << " { NID=" << s.first << ", Criteria=" << s.second.criteria << "},";
	}
	ss << " ], Root=[";
	for (const auto& s : root_criteria) {
		ss << " { NID=" << s.first << ", Criteria=" << s.second.criteria << "},";
	}
	ss << " ] }";

	return ss.str();
}

static int hexValue(char c) {
	static char const digits[] = "0123456789ABCDEF";

	if (c >= 'a' && c <= 'f')
		c -= ('a' - 'A');

	int value = std::find(digits, digits + 16, c) - digits;
	if (value >= 16) {
		throw std::runtime_error("hexValue");
	}
	return value;
}

// Does not handle "raw" form (e.g. #28C4D1), only escaped text
static std::string de4514(std::string const& input, int start, int& out_end) {
	std::string output;

	if (input[start] == '#' || input[start] == ' ') {
		out_end = start;
		return output;
	}

	int space_count = 0;

	for (int p = start; p < input.size();) {
		switch (input[p]) {
		case '\\': // Handle escaped sequence

			// Backslash escaping nothing!
			if (p == input.size() - 1) {
				out_end = p;
				goto FIN;
			}

			switch (input[p + 1]) {
			case ' ':
			case '"':
			case '#':
			case '+':
			case ',':
			case ';':
			case '<':
			case '=':
			case '>':
			case '|':
			case '\\':
				output += input[p + 1];
				p += 2;
				space_count = 0;
				continue;

			default:
				// Backslash escaping pair of hex digits requires two characters
				if (p == input.size() - 2) {
					out_end = p;
					goto FIN;
				}

				try {
					output += hexValue(input[p + 1]) * 16 + hexValue(input[p + 2]);
					p += 3;
					space_count = 0;
					continue;
				} catch (...) {
					out_end = p;
					goto FIN;
				}
			}

		case '"':
		case '+':
		case ',':
		case ';':
		case '<':
		case '>':
		case 0:
			// All of these must have been escaped
			out_end = p;
			goto FIN;

		default:
			// Character is what it is
			output += input[p];
			if (input[p] == ' ')
				space_count++;
			else
				space_count = 0;
			p++;
		}
	}

	out_end = input.size();

FIN:
	out_end -= space_count;
	output.resize(output.size() - space_count);

	return output;
}

static std::pair<std::string, std::string> splitPair(std::string const& input, char c) {
	int p = input.find_first_of(c);
	if (p == input.npos) {
		throw std::runtime_error("splitPair");
	}
	return std::make_pair(input.substr(0, p), input.substr(p + 1, input.size()));
}

static NID abbrevToNID(std::string const& sn) {
	NID nid = NID_undef;

	if (sn == "C" || sn == "CN" || sn == "L" || sn == "ST" || sn == "O" || sn == "OU" || sn == "UID" || sn == "DC" ||
	    sn == "subjectAltName")
		nid = OBJ_sn2nid(sn.c_str());
	if (nid == NID_undef)
		throw std::runtime_error("abbrevToNID");

	return nid;
}

static X509Location locationForNID(NID nid) {
	const char* name = OBJ_nid2ln(nid);
	if (name == nullptr) {
		throw std::runtime_error("locationForNID");
	}
	if (strncmp(name, "X509v3", 6) == 0) {
		return X509Location::EXTENSION;
	} else {
		// It probably isn't true that all other NIDs live in the NAME, but it is for now...
		return X509Location::NAME;
	}
}

void TLSPolicy::set_verify_peers(std::vector<std::string> verify_peers) {
	for (int i = 0; i < verify_peers.size(); i++) {
		try {
			std::string& verifyString = verify_peers[i];
			int start = 0;
			while (start < verifyString.size()) {
				int split = verifyString.find('|', start);
				if (split == std::string::npos) {
					break;
				}
				if (split == start || verifyString[split - 1] != '\\') {
					rules.emplace_back(verifyString.substr(start, split - start));
					start = split + 1;
				}
			}
			rules.emplace_back(verifyString.substr(start));
		} catch (const std::runtime_error& e) {
			rules.clear();
			std::string& verifyString = verify_peers[i];
			TraceEvent(SevError, "FDBLibTLSVerifyPeersParseError").detail("Config", verifyString);
			throw tls_error();
		}
	}
}

TLSPolicy::Rule::Rule(std::string input) {
	int s = 0;

	while (s < input.size()) {
		int eq = input.find('=', s);

		if (eq == input.npos)
			throw std::runtime_error("parse_verify");

		MatchType mt = MatchType::EXACT;
		if (input[eq - 1] == '>')
			mt = MatchType::PREFIX;
		if (input[eq - 1] == '<')
			mt = MatchType::SUFFIX;
		std::string term = input.substr(s, eq - s - (mt == MatchType::EXACT ? 0 : 1));

		if (term.find("Check.") == 0) {
			if (eq + 2 > input.size())
				throw std::runtime_error("parse_verify");
			if (eq + 2 != input.size() && input[eq + 2] != ',')
				throw std::runtime_error("parse_verify");
			if (mt != MatchType::EXACT)
				throw std::runtime_error("parse_verify: cannot prefix match Check");

			bool* flag;

			if (term == "Check.Valid")
				flag = &verify_cert;
			else if (term == "Check.Unexpired")
				flag = &verify_time;
			else
				throw std::runtime_error("parse_verify");

			if (input[eq + 1] == '0')
				*flag = false;
			else if (input[eq + 1] == '1')
				*flag = true;
			else
				throw std::runtime_error("parse_verify");

			s = eq + 3;
		} else {
			std::map<int, Criteria>* criteria = &subject_criteria;

			if (term.find('.') != term.npos) {
				auto scoped = splitPair(term, '.');

				if (scoped.first == "S" || scoped.first == "Subject")
					criteria = &subject_criteria;
				else if (scoped.first == "I" || scoped.first == "Issuer")
					criteria = &issuer_criteria;
				else if (scoped.first == "R" || scoped.first == "Root")
					criteria = &root_criteria;
				else
					throw std::runtime_error("parse_verify");

				term = scoped.second;
			}

			int remain;
			auto unesc = de4514(input, eq + 1, remain);

			if (remain == eq + 1)
				throw std::runtime_error("parse_verify");

			NID termNID = abbrevToNID(term);
			const X509Location loc = locationForNID(termNID);
			criteria->insert(std::make_pair(termNID, Criteria(unesc, mt, loc)));

			if (remain != input.size() && input[remain] != ',')
				throw std::runtime_error("parse_verify");

			s = remain + 1;
		}
	}
}

bool match_criteria_entry(const std::string& criteria, ASN1_STRING* entry, MatchType mt) {
	bool rc = false;
	ASN1_STRING* asn_criteria = nullptr;
	unsigned char* criteria_utf8 = nullptr;
	int criteria_utf8_len = 0;
	unsigned char* entry_utf8 = nullptr;
	int entry_utf8_len = 0;

	if ((asn_criteria = ASN1_IA5STRING_new()) == nullptr)
		goto err;
	if (ASN1_STRING_set(asn_criteria, criteria.c_str(), criteria.size()) != 1)
		goto err;
	if ((criteria_utf8_len = ASN1_STRING_to_UTF8(&criteria_utf8, asn_criteria)) < 1)
		goto err;
	if ((entry_utf8_len = ASN1_STRING_to_UTF8(&entry_utf8, entry)) < 1)
		goto err;
	if (mt == MatchType::EXACT) {
		if (criteria_utf8_len == entry_utf8_len && memcmp(criteria_utf8, entry_utf8, criteria_utf8_len) == 0)
			rc = true;
	} else if (mt == MatchType::PREFIX) {
		if (criteria_utf8_len <= entry_utf8_len && memcmp(criteria_utf8, entry_utf8, criteria_utf8_len) == 0)
			rc = true;
	} else if (mt == MatchType::SUFFIX) {
		if (criteria_utf8_len <= entry_utf8_len &&
		    memcmp(criteria_utf8, entry_utf8 + (entry_utf8_len - criteria_utf8_len), criteria_utf8_len) == 0)
			rc = true;
	}

err:
	ASN1_STRING_free(asn_criteria);
	free(criteria_utf8);
	free(entry_utf8);
	return rc;
}

bool match_name_criteria(X509_NAME* name, NID nid, const std::string& criteria, MatchType mt) {
	X509_NAME_ENTRY* name_entry;
	int idx;

	// If name does not exist, or has multiple of this RDN, refuse to proceed.
	if ((idx = X509_NAME_get_index_by_NID(name, nid, -1)) < 0)
		return false;
	if (X509_NAME_get_index_by_NID(name, nid, idx) != -1)
		return false;
	if ((name_entry = X509_NAME_get_entry(name, idx)) == nullptr)
		return false;

	return match_criteria_entry(criteria, X509_NAME_ENTRY_get_data(name_entry), mt);
}

bool match_extension_criteria(X509* cert, NID nid, const std::string& value, MatchType mt) {
	if (nid != NID_subject_alt_name && nid != NID_issuer_alt_name) {
		// I have no idea how other extensions work.
		return false;
	}
	auto pos = value.find(':');
	if (pos == value.npos) {
		return false;
	}
	std::string value_gen = value.substr(0, pos);
	std::string value_val = value.substr(pos + 1, value.npos);
	STACK_OF(GENERAL_NAME)* sans =
	    reinterpret_cast<STACK_OF(GENERAL_NAME)*>(X509_get_ext_d2i(cert, nid, nullptr, nullptr));
	if (sans == nullptr) {
		return false;
	}
	int num_sans = sk_GENERAL_NAME_num(sans);
	bool rc = false;
	for (int i = 0; i < num_sans && !rc; ++i) {
		GENERAL_NAME* altname = sk_GENERAL_NAME_value(sans, i);
		std::string matchable;
		switch (altname->type) {
		case GEN_OTHERNAME:
			break;
		case GEN_EMAIL:
			if (value_gen == "EMAIL" && match_criteria_entry(value_val, altname->d.rfc822Name, mt)) {
				rc = true;
				break;
			}
		case GEN_DNS:
			if (value_gen == "DNS" && match_criteria_entry(value_val, altname->d.dNSName, mt)) {
				rc = true;
				break;
			}
		case GEN_X400:
		case GEN_DIRNAME:
		case GEN_EDIPARTY:
			break;
		case GEN_URI:
			if (value_gen == "URI" && match_criteria_entry(value_val, altname->d.uniformResourceIdentifier, mt)) {
				rc = true;
				break;
			}
		case GEN_IPADD:
			if (value_gen == "IP" && match_criteria_entry(value_val, altname->d.iPAddress, mt)) {
				rc = true;
				break;
			}
		case GEN_RID:
			break;
		}
	}
	sk_GENERAL_NAME_pop_free(sans, GENERAL_NAME_free);
	return rc;
}

bool match_criteria(X509* cert,
                    X509_NAME* subject,
                    NID nid,
                    const std::string& criteria,
                    MatchType mt,
                    X509Location loc) {
	switch (loc) {
	case X509Location::NAME: {
		return match_name_criteria(subject, nid, criteria, mt);
	}
	case X509Location::EXTENSION: {
		return match_extension_criteria(cert, nid, criteria, mt);
	}
	}
	// Should never be reachable.
	return false;
}

std::tuple<bool, std::string> check_verify(const TLSPolicy::Rule* verify, X509_STORE_CTX* store_ctx, bool is_client) {
	X509_NAME *subject, *issuer;
	bool rc = false;
	X509* cert = nullptr;
	// if returning false, give a reason string
	std::string reason = "";

	// Check subject criteria.
	cert = sk_X509_value(X509_STORE_CTX_get0_chain(store_ctx), 0);
	if ((subject = X509_get_subject_name(cert)) == nullptr) {
		reason = "Cert subject error";
		goto err;
	}
	for (auto& pair : verify->subject_criteria) {
		if (!match_criteria(
		        cert, subject, pair.first, pair.second.criteria, pair.second.match_type, pair.second.location)) {
			reason = "Cert subject match failure";
			goto err;
		}
	}

	// Check issuer criteria.
	if ((issuer = X509_get_issuer_name(cert)) == nullptr) {
		reason = "Cert issuer error";
		goto err;
	}
	for (auto& pair : verify->issuer_criteria) {
		if (!match_criteria(
		        cert, issuer, pair.first, pair.second.criteria, pair.second.match_type, pair.second.location)) {
			reason = "Cert issuer match failure";
			goto err;
		}
	}

	// Check root criteria - this is the subject of the final certificate in the stack.
	cert = sk_X509_value(X509_STORE_CTX_get0_chain(store_ctx), sk_X509_num(X509_STORE_CTX_get0_chain(store_ctx)) - 1);
	if ((subject = X509_get_subject_name(cert)) == nullptr) {
		reason = "Root subject error";
		goto err;
	}
	for (auto& pair : verify->root_criteria) {
		if (!match_criteria(
		        cert, subject, pair.first, pair.second.criteria, pair.second.match_type, pair.second.location)) {
			reason = "Root subject match failure";
			goto err;
		}
	}

	// If we got this far, everything checked out...
	rc = true;

err:
	return std::make_tuple(rc, reason);
}

bool TLSPolicy::verify_peer(bool preverified, X509_STORE_CTX* store_ctx) {
	bool rc = false;
	std::set<std::string> verify_failure_reasons;
	bool verify_success;
	std::string verify_failure_reason;

	// If certificate verification is disabled, there's nothing more to do.
	if (std::any_of(rules.begin(), rules.end(), [](const Rule& r) { return !r.verify_cert; })) {
		return true;
	}

	if (!preverified) {
		TraceEvent(SevWarn, "TLSPolicyFailure")
		    .suppressFor(1.0)
		    .detail("Reason", "preverification failed")
		    .detail("VerifyError", X509_verify_cert_error_string(X509_STORE_CTX_get_error(store_ctx)));
		return false;
	}

	if (!rules.size()) {
		return true;
	}

	// Any matching rule is sufficient.
	for (auto& verify_rule : rules) {
		std::tie(verify_success, verify_failure_reason) = check_verify(&verify_rule, store_ctx, is_client);
		if (verify_success) {
			rc = true;
			break;
		} else {
			if (verify_failure_reason.length() > 0)
				verify_failure_reasons.insert(verify_failure_reason);
		}
	}

	if (!rc) {
		// log the various failure reasons
		for (std::string reason : verify_failure_reasons) {
			TraceEvent(SevWarn, "TLSPolicyFailure").suppressFor(1.0).detail("Reason", reason);
		}
	}
	return rc;
}
#endif
