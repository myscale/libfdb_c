#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
/*
 * GenericManagementAPI.actor.h
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

#pragma once
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_G_H)
#define FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_G_H
#include "fdbclient/GenericManagementAPI.actor.g.h"
#elif !defined(FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_H)
#define FDBCLIENT_GENERIC_MANAGEMENT_API_ACTOR_H

/* This file defines "management" interfaces that have been templated to support both IClientAPI
and Native version of databases, transactions, etc., and includes functions for performing cluster
managment tasks. It isn't exposed to C clients or anywhere outside our code base and doesn't need
to be versioned. It doesn't do anything you can't do with the standard API and some knowledge of
the contents of the system key space.
*/

#include <string>
#include <map>
#include "fdbclient/ClientBooleanParams.h"
#include "fdbclient/DatabaseConfiguration.h"
#include "fdbclient/Status.h"
#include "fdbclient/Subspace.h"
#include "fdbclient/DatabaseConfiguration.h"
#include "fdbclient/Status.h"
#include "fdbclient/SystemData.h"
#include "flow/actorcompiler.h" // has to be last include

// ConfigurationResult enumerates normal outcomes of changeConfig() and various error
// conditions specific to it.  changeConfig may also throw an Error to report other problems.
enum class ConfigurationResult {
	NO_OPTIONS_PROVIDED,
	CONFLICTING_OPTIONS,
	UNKNOWN_OPTION,
	INCOMPLETE_CONFIGURATION,
	INVALID_CONFIGURATION,
	STORAGE_MIGRATION_DISABLED,
	DATABASE_ALREADY_CREATED,
	DATABASE_CREATED,
	DATABASE_UNAVAILABLE,
	STORAGE_IN_UNKNOWN_DCID,
	REGION_NOT_FULLY_REPLICATED,
	MULTIPLE_ACTIVE_REGIONS,
	REGIONS_CHANGED,
	NOT_ENOUGH_WORKERS,
	REGION_REPLICATION_MISMATCH,
	DCID_MISSING,
	LOCKED_NOT_NEW,
	SUCCESS_WARN_PPW_GRADUAL,
	SUCCESS,
	SUCCESS_WARN_ROCKSDB_EXPERIMENTAL,
	DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL,
};

enum class CoordinatorsResult {
	INVALID_NETWORK_ADDRESSES,
	SAME_NETWORK_ADDRESSES,
	NOT_COORDINATORS, // FIXME: not detected
	DATABASE_UNREACHABLE, // FIXME: not detected
	BAD_DATABASE_STATE,
	COORDINATOR_UNREACHABLE,
	NOT_ENOUGH_MACHINES,
	SUCCESS
};

struct ConfigureAutoResult {
	std::map<NetworkAddress, ProcessClass> address_class;
	int32_t processes;
	int32_t machines;

	std::string old_replication;
	int32_t old_commit_proxies;
	int32_t old_grv_proxies;
	int32_t old_resolvers;
	int32_t old_logs;
	int32_t old_processes_with_transaction;
	int32_t old_machines_with_transaction;

	std::string auto_replication;
	int32_t auto_commit_proxies;
	int32_t auto_grv_proxies;
	int32_t auto_resolvers;
	int32_t auto_logs;
	int32_t auto_processes_with_transaction;
	int32_t auto_machines_with_transaction;

	int32_t desired_commit_proxies;
	int32_t desired_grv_proxies;
	int32_t desired_resolvers;
	int32_t desired_logs;

	ConfigureAutoResult()
	  : processes(-1), machines(-1), old_commit_proxies(-1), old_grv_proxies(-1), old_resolvers(-1), old_logs(-1),
	    old_processes_with_transaction(-1), old_machines_with_transaction(-1), auto_commit_proxies(-1),
	    auto_grv_proxies(-1), auto_resolvers(-1), auto_logs(-1), auto_processes_with_transaction(-1),
	    auto_machines_with_transaction(-1), desired_commit_proxies(-1), desired_grv_proxies(-1), desired_resolvers(-1),
	    desired_logs(-1) {}

	bool isValid() const { return processes != -1; }
};

ConfigurationResult buildConfiguration(
    std::vector<StringRef> const& modeTokens,
    std::map<std::string, std::string>& outConf); // Accepts a vector of configuration tokens
ConfigurationResult buildConfiguration(
    std::string const& modeString,
    std::map<std::string, std::string>& outConf); // Accepts tokens separated by spaces in a single string

bool isCompleteConfiguration(std::map<std::string, std::string> const& options);

ConfigureAutoResult parseConfig(StatusObject const& status);

template <typename Transaction, class T>
struct transaction_future_type {
	using type = typename Transaction::template FutureT<T>;
};

template <typename Transaction, class T>
struct transaction_future_type<Transaction*, T> {
	using type = typename transaction_future_type<Transaction, T>::type;
};

template <typename Transaction, class T>
struct transaction_future_type<Reference<Transaction>, T> {
	using type = typename transaction_future_type<Transaction, T>::type;
};

// Management API written in template code to support both IClientAPI and NativeAPI
namespace ManagementAPI {

															#line 150 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via changeCachedRange()
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class ChangeCachedRangeActor>
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ChangeCachedRangeActorState {
															#line 156 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ChangeCachedRangeActorState(Reference<DB> const& db,KeyRangeRef const& range,bool const& add) 
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   range(range),
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   add(add),
															#line 150 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 151 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   sysRange(KeyRangeRef(storageCacheKey(range.begin), storageCacheKey(range.end))),
															#line 152 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   sysRangeClear(KeyRangeRef(storageCacheKey(range.begin), keyAfter(storageCacheKey(range.end)))),
															#line 153 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   privateRange(KeyRangeRef(cacheKeysKey(0, range.begin), cacheKeysKey(0, range.end))),
															#line 154 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   trueValue(storageCacheValue(std::vector<uint16_t>{ 0 })),
															#line 155 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   falseValue(storageCacheValue(std::vector<uint16_t>{}))
															#line 179 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("changeCachedRange", reinterpret_cast<unsigned long>(this));

	}
	~ChangeCachedRangeActorState() 
	{
		fdb_probe_actor_destroy("changeCachedRange", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 156 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 194 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
															#line 157 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 158 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 226 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		try {
															#line 160 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->clear(sysRangeClear);
															#line 161 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->clear(privateRange);
															#line 162 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->addReadConflictRange(privateRange);
															#line 164 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			previousFuture = tr->getRange(KeyRangeRef(storageCachePrefix, sysRange.begin), 1, Snapshot::False, Reverse::True);
															#line 166 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(previousFuture);
															#line 166 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 240 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 1;
															#line 166 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 245 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 202 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			err = e;
															#line 203 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 203 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 271 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 4;
															#line 203 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 3, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 276 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult const& previous,int loopDepth) 
	{
															#line 167 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		bool prevIsCached = false;
															#line 168 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!previous.empty())
															#line 293 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 169 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> prevVal;
															#line 170 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(previous[0].value, prevVal);
															#line 171 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			prevIsCached = !prevVal.empty();
															#line 301 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 173 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (prevIsCached && !add)
															#line 305 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 175 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.begin, falseValue);
															#line 176 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.begin, serverKeysFalse);
															#line 311 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 177 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!prevIsCached && add)
															#line 317 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 179 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.begin, trueValue);
															#line 180 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.begin, serverKeysTrue);
															#line 323 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 183 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		afterFuture = tr->getRange(KeyRangeRef(sysRange.end, storageCacheKeys.end), 1, Snapshot::False, Reverse::False);
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(afterFuture);
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 332 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 2;
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 337 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(RangeResult && previous,int loopDepth) 
	{
															#line 167 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		bool prevIsCached = false;
															#line 168 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!previous.empty())
															#line 348 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 169 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> prevVal;
															#line 170 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(previous[0].value, prevVal);
															#line 171 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			prevIsCached = !prevVal.empty();
															#line 356 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 173 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (prevIsCached && !add)
															#line 360 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 175 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.begin, falseValue);
															#line 176 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.begin, serverKeysFalse);
															#line 366 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 177 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!prevIsCached && add)
															#line 372 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 179 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.begin, trueValue);
															#line 180 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.begin, serverKeysTrue);
															#line 378 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 183 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		afterFuture = tr->getRange(KeyRangeRef(sysRange.end, storageCacheKeys.end), 1, Snapshot::False, Reverse::False);
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(afterFuture);
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 387 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 2;
															#line 185 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 392 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult const& previous,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(previous, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(RangeResult && previous,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(previous), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(RangeResult const& after,int loopDepth) 
	{
															#line 186 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		bool afterIsCached = false;
															#line 187 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!after.empty())
															#line 466 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 188 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> afterVal;
															#line 189 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(after[0].value, afterVal);
															#line 190 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			afterIsCached = afterVal.empty();
															#line 474 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 192 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (afterIsCached && !add)
															#line 478 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 193 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.end, trueValue);
															#line 194 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.end, serverKeysTrue);
															#line 484 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 195 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!afterIsCached && add)
															#line 490 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 196 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.end, falseValue);
															#line 197 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.end, serverKeysFalse);
															#line 496 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 503 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 3;
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 2, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 508 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(RangeResult && after,int loopDepth) 
	{
															#line 186 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		bool afterIsCached = false;
															#line 187 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!after.empty())
															#line 519 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 188 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<uint16_t> afterVal;
															#line 189 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			decodeStorageCacheValue(after[0].value, afterVal);
															#line 190 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			afterIsCached = afterVal.empty();
															#line 527 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 192 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (afterIsCached && !add)
															#line 531 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 193 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(sysRange.end, trueValue);
															#line 194 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(privateRange.end, serverKeysTrue);
															#line 537 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 195 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!afterIsCached && add)
															#line 543 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 196 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(sysRange.end, falseValue);
															#line 197 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(privateRange.end, serverKeysFalse);
															#line 549 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 3;
															#line 199 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeCachedRangeActor, 2, Void >*>(static_cast<ChangeCachedRangeActor*>(this)));
															#line 561 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult const& after,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(after, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(RangeResult && after,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(after), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont8(Void const& _,int loopDepth) 
	{
															#line 200 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ChangeCachedRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeCachedRangeActorState(); static_cast<ChangeCachedRangeActor*>(this)->destroy(); return 0; }
															#line 633 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ChangeCachedRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont8(Void && _,int loopDepth) 
	{
															#line 200 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ChangeCachedRangeActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~ChangeCachedRangeActorState(); static_cast<ChangeCachedRangeActor*>(this)->destroy(); return 0; }
															#line 645 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ChangeCachedRangeActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~ChangeCachedRangeActorState();
		static_cast<ChangeCachedRangeActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont8(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
															#line 204 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent(SevDebug, "ChangeCachedRangeError").error(err);
															#line 720 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
															#line 204 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent(SevDebug, "ChangeCachedRangeError").error(err);
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state > 0) static_cast<ChangeCachedRangeActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeCachedRangeActor*>(this)->ActorCallback< ChangeCachedRangeActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeCachedRangeActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	KeyRangeRef range;
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool add;
															#line 150 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 151 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	KeyRange sysRange;
															#line 152 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	KeyRange sysRangeClear;
															#line 153 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	KeyRange privateRange;
															#line 154 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Value trueValue;
															#line 155 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Value falseValue;
															#line 164 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> previousFuture;
															#line 183 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> afterFuture;
															#line 202 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Error err;
															#line 821 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via changeCachedRange()
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ChangeCachedRangeActor final : public Actor<Void>, public ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >, public ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >, public ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >, public ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >, public FastAllocated<ChangeCachedRangeActor<DB>>, public ChangeCachedRangeActorState<DB, ChangeCachedRangeActor<DB>> {
															#line 828 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ChangeCachedRangeActor<DB>>::operator new;
	using FastAllocated<ChangeCachedRangeActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >;
friend struct ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >;
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ChangeCachedRangeActor(Reference<DB> const& db,KeyRangeRef const& range,bool const& add) 
															#line 842 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Void>(),
		   ChangeCachedRangeActorState<DB, ChangeCachedRangeActor<DB>>(db, range, add)
	{
		fdb_probe_actor_enter("changeCachedRange", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeCachedRange");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeCachedRange", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 0, RangeResult >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 1, RangeResult >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ChangeCachedRangeActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Void> changeCachedRange( Reference<DB> const& db, KeyRangeRef const& range, bool const& add ) {
															#line 148 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Void>(new ChangeCachedRangeActor<DB>(db, range, add));
															#line 874 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 208 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

template <class DB>
Future<Void> addCachedRange(Reference<DB> db, KeyRangeRef range) {
	return changeCachedRange(db, range, true);
}

template <class DB>
Future<Void> removeCachedRange(Reference<DB> db, KeyRangeRef range) {
	return changeCachedRange(db, range, false);
}

															#line 889 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via getWorkers()
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Tr, class GetWorkersActor>
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetWorkersActorState {
															#line 895 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetWorkersActorState(Reference<Tr> const& tr,typename Tr::template FutureT<RangeResult> const& processClassesF,typename Tr::template FutureT<RangeResult> const& processDataF) 
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   processClassesF(processClassesF),
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   processDataF(processDataF)
															#line 906 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	~GetWorkersActorState() 
	{
		fdb_probe_actor_destroy("getWorkers", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 224 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processClassesF = tr->getRange(processClassKeys, CLIENT_KNOBS->TOO_MANY);
															#line 225 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processDataF = tr->getRange(workerListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 226 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processClasses = safeThreadFutureToFuture(processClassesF);
															#line 227 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processData = safeThreadFutureToFuture(processDataF);
															#line 229 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_0 = success(processClasses) && success(processData);
															#line 229 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<GetWorkersActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 931 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetWorkersActor*>(this)->actor_wait_state = 1;
															#line 229 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetWorkersActor, 0, Void >*>(static_cast<GetWorkersActor*>(this)));
															#line 936 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 231 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 233 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 234 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 235 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 967 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 239 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::vector<ProcessData> results;
															#line 241 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processData.get().size();i++) {
															#line 242 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 243 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 245 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 979 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 247 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				data.processClass = processClass;
															#line 983 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 249 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 987 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 250 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				results.push_back(data);
															#line 991 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 253 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 996 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetWorkersActor*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(results);
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 230 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processClasses.get().more && processClasses.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 231 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!processData.get().more && processData.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 233 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<Optional<Standalone<StringRef>>, ProcessClass> id_class;
															#line 234 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processClasses.get().size();i++) {
															#line 235 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			id_class[decodeProcessClassKey(processClasses.get()[i].key)] = decodeProcessClassValue(processClasses.get()[i].value);
															#line 1016 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 239 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::vector<ProcessData> results;
															#line 241 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for(int i = 0;i < processData.get().size();i++) {
															#line 242 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ProcessData data = decodeWorkerListValue(processData.get()[i].value);
															#line 243 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ProcessClass processClass = id_class[data.locality.processId()];
															#line 245 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (processClass.classSource() == ProcessClass::DBSource || data.processClass.classType() == ProcessClass::UnsetClass)
															#line 1028 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 247 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				data.processClass = processClass;
															#line 1032 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 249 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (data.processClass.classType() != ProcessClass::TesterClass)
															#line 1036 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 250 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				results.push_back(data);
															#line 1040 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 253 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetWorkersActor*>(this)->SAV<std::vector<ProcessData>>::futures) { (void)(results); this->~GetWorkersActorState(); static_cast<GetWorkersActor*>(this)->destroy(); return 0; }
															#line 1045 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetWorkersActor*>(this)->SAV< std::vector<ProcessData> >::value()) std::vector<ProcessData>(results);
		this->~GetWorkersActorState();
		static_cast<GetWorkersActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetWorkersActor*>(this)->actor_wait_state > 0) static_cast<GetWorkersActor*>(this)->actor_wait_state = 0;
		static_cast<GetWorkersActor*>(this)->ActorCallback< GetWorkersActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< GetWorkersActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetWorkersActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetWorkersActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<Tr> tr;
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename Tr::template FutureT<RangeResult> processClassesF;
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename Tr::template FutureT<RangeResult> processDataF;
															#line 226 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> processClasses;
															#line 227 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> processData;
															#line 1126 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via getWorkers()
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Tr>
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetWorkersActor final : public Actor<std::vector<ProcessData>>, public ActorCallback< GetWorkersActor<Tr>, 0, Void >, public FastAllocated<GetWorkersActor<Tr>>, public GetWorkersActorState<Tr, GetWorkersActor<Tr>> {
															#line 1133 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<GetWorkersActor<Tr>>::operator new;
	using FastAllocated<GetWorkersActor<Tr>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::vector<ProcessData>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetWorkersActor<Tr>, 0, Void >;
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetWorkersActor(Reference<Tr> const& tr,typename Tr::template FutureT<RangeResult> const& processClassesF,typename Tr::template FutureT<RangeResult> const& processDataF) 
															#line 1144 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<std::vector<ProcessData>>(),
		   GetWorkersActorState<Tr, GetWorkersActor<Tr>>(tr, processClassesF, processDataF)
	{
		fdb_probe_actor_enter("getWorkers", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getWorkers");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getWorkers", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetWorkersActor<Tr>, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Tr>
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Reference<Tr> const& tr, typename Tr::template FutureT<RangeResult> const& processClassesF, typename Tr::template FutureT<RangeResult> const& processDataF ) {
															#line 219 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<std::vector<ProcessData>>(new GetWorkersActor<Tr>(tr, processClassesF, processDataF));
															#line 1173 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 255 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

// All versions of changeConfig apply the given set of configuration tokens to the database, and return a
// ConfigurationResult (or error).

// Accepts a full configuration in key/value format (from buildConfiguration)
															#line 1182 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via changeConfig()
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class ChangeConfigActor>
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ChangeConfigActorState {
															#line 1188 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ChangeConfigActorState(Reference<DB> const& db,std::map<std::string, std::string> const& m,bool const& force) 
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   m(m),
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   force(force),
															#line 262 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   initIdKey(LiteralStringRef("\xff/init_id")),
															#line 263 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction())
															#line 1203 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("changeConfig", reinterpret_cast<unsigned long>(this));

	}
	~ChangeConfigActorState() 
	{
		fdb_probe_actor_destroy("changeConfig", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 265 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!m.size())
															#line 1218 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 266 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NO_OPTIONS_PROVIDED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1222 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NO_OPTIONS_PROVIDED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 270 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::string initKey = configKeysPrefix.toString() + "initialized";
															#line 271 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			creating = m.count(initKey) != 0;
															#line 272 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			locked = Optional<UID>();
															#line 1234 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 274 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				auto iter = m.find(databaseLockedKey.toString());
															#line 275 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (iter != m.end())
															#line 1240 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 276 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!creating)
															#line 1244 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 277 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::LOCKED_NOT_NEW); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1248 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::LOCKED_NOT_NEW);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
															#line 279 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					locked = UID::fromString(iter->second);
															#line 280 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					m.erase(iter);
															#line 1258 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 283 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (creating)
															#line 1263 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 284 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				m[initIdKey.toString()] = deterministicRandom()->randomUniqueID().toString();
															#line 285 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!isCompleteConfiguration(m))
															#line 1269 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 286 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INCOMPLETE_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1273 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INCOMPLETE_CONFIGURATION);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 290 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tooLong = delay(60);
															#line 291 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			versionKey = BinaryWriter::toValue(deterministicRandom()->randomUniqueID(), Unversioned());
															#line 292 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = false;
															#line 293 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			warnPPWGradual = false;
															#line 294 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			warnRocksDBIsExperimental = false;
															#line 295 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 1292 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~ChangeConfigActorState();
		static_cast<ChangeConfigActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 547 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (warnPPWGradual)
															#line 1315 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 548 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS_WARN_PPW_GRADUAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1319 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS_WARN_PPW_GRADUAL);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 549 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (warnRocksDBIsExperimental)
															#line 1329 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 550 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1333 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 552 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1343 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 297 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 298 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 299 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 300 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 302 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!creating && !force)
															#line 1373 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 303 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				fConfigF = tr->getRange(configKeys, CLIENT_KNOBS->TOO_MANY);
															#line 305 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				fConfig = safeThreadFutureToFuture(fConfigF);
															#line 306 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				processClassesF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 307 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				processDataF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 308 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				fWorkers = getWorkers(tr, processClassesF, processDataF);
															#line 309 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				StrictFuture<Void> __when_expr_0 = success(fConfig) || tooLong;
															#line 309 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1389 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ChangeConfigActor*>(this)->actor_wait_state = 1;
															#line 309 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 0, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1394 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1break1(int loopDepth) 
	{
		try {
			return a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 517 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			e1 = Error(e);
															#line 518 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if ((e.code() == error_code_not_committed || e.code() == error_code_transaction_too_old) && creating)
															#line 1436 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 521 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->reset();
															#line 522 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				;
															#line 1442 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = a_body1loopBody1Catch1loopHead1(loopDepth);
			}
			else
			{
				loopDepth = a_body1loopBody1Catch1cont1(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 488 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (creating)
															#line 1462 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 489 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::INITIALIZE_NEW_DATABASE);
															#line 490 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->addReadConflictRange(singleKeyRange(initIdKey));
															#line 1468 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
		else
		{
															#line 491 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (m.size())
															#line 1474 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 494 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->setOption(FDBTransactionOptions::CAUSAL_WRITE_RISKY);
															#line 495 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->addReadConflictRange(singleKeyRange(m.begin()->first));
															#line 1480 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 498 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (locked.present())
															#line 1485 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 499 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(creating);
															#line 500 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->atomicOp(databaseLockedKey, BinaryWriter::toValue(locked.get(), Unversioned()) .withPrefix(LiteralStringRef("0123456789")) .withSuffix(LiteralStringRef("\x00\x00\x00\x00")), MutationRef::SetVersionstampedValue);
															#line 1491 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 507 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for(auto i = m.begin();i != m.end();++i) {
															#line 508 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(StringRef(i->first), StringRef(i->second));
															#line 1497 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 511 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 512 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 514 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_5 = safeThreadFutureToFuture(tr->commit());
															#line 514 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1507 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1loopBody1Catch1(__when_expr_5.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_5.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 6;
															#line 514 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 5, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1512 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 311 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fConfig.isReady())
															#line 1521 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 312 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1525 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 315 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (fConfig.isReady())
															#line 1533 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 316 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(fConfig.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 317 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldConfig = DatabaseConfiguration();
															#line 318 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldConfig.fromKeyValues((VectorRef<KeyValueRef>)fConfig.get());
															#line 319 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			newConfig = oldConfig;
															#line 320 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto kv : m ) {
															#line 321 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				newConfig.set(kv.first, kv.second);
															#line 1547 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 323 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!newConfig.isValid())
															#line 1551 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 324 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INVALID_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1555 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INVALID_CONFIGURATION);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 327 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.tLogPolicy->attributeKeys().count("dcid") && newConfig.regions.size() > 0)
															#line 1563 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 328 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_REPLICATION_MISMATCH); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1567 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_REPLICATION_MISMATCH);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 331 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = oldReplicationUsesDcId || oldConfig.tLogPolicy->attributeKeys().count("dcid");
															#line 334 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (oldConfig.usableRegions != newConfig.usableRegions)
															#line 1577 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 336 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				std::map<Key, int32_t> dcId_priority;
															#line 337 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 338 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					dcId_priority[it.dcId] = it.priority;
															#line 1585 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
															#line 340 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : oldConfig.regions ) {
															#line 341 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!dcId_priority.count(it.dcId) || dcId_priority[it.dcId] != it.priority)
															#line 1591 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 342 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGIONS_CHANGED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1595 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGIONS_CHANGED);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
															#line 347 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				int activeRegionCount = 0;
															#line 348 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 349 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (it.priority >= 0)
															#line 1608 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 350 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						activeRegionCount++;
															#line 1612 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
															#line 353 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (activeRegionCount > 1)
															#line 1617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 354 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1621 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 358 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			fServerListF = tr->getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 360 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			fServerList = (newConfig.regions.size()) ? safeThreadFutureToFuture(fServerListF) : Future<RangeResult>();
															#line 363 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.usableRegions == 2)
															#line 1634 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 364 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (oldReplicationUsesDcId)
															#line 1638 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 365 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					fLocalityListF = tr->getRange(tagLocalityListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 367 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					fLocalityList = safeThreadFutureToFuture(fLocalityListF);
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_1 = success(fLocalityList) || tooLong;
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1648 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 2;
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 1, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1653 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
				else
				{
															#line 390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					replicasFuturesF = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 392 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 393 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					for( auto& it : newConfig.regions ) {
															#line 394 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (it.priority >= 0)
															#line 1666 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						{
															#line 395 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
							replicasFuturesF.push_back(tr->get(datacenterReplicasKeyFor(it.dcId)));
															#line 396 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
							replicasFutures.push_back(safeThreadFutureToFuture(replicasFuturesF.back()));
															#line 1672 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						}
					}
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_2 = waitForAll(replicasFutures) || tooLong;
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1679 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when2(__when_expr_2.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 3;
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 2, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
			}
			else
			{
				loopDepth = a_body1loopBody1cont6(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 311 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fConfig.isReady())
															#line 1704 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 312 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1708 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 315 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (fConfig.isReady())
															#line 1716 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 316 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			ASSERT(fConfig.get().size() < CLIENT_KNOBS->TOO_MANY);
															#line 317 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldConfig = DatabaseConfiguration();
															#line 318 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldConfig.fromKeyValues((VectorRef<KeyValueRef>)fConfig.get());
															#line 319 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			newConfig = oldConfig;
															#line 320 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto kv : m ) {
															#line 321 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				newConfig.set(kv.first, kv.second);
															#line 1730 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 323 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!newConfig.isValid())
															#line 1734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 324 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INVALID_CONFIGURATION); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1738 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INVALID_CONFIGURATION);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 327 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.tLogPolicy->attributeKeys().count("dcid") && newConfig.regions.size() > 0)
															#line 1746 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 328 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_REPLICATION_MISMATCH); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1750 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_REPLICATION_MISMATCH);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 331 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			oldReplicationUsesDcId = oldReplicationUsesDcId || oldConfig.tLogPolicy->attributeKeys().count("dcid");
															#line 334 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (oldConfig.usableRegions != newConfig.usableRegions)
															#line 1760 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 336 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				std::map<Key, int32_t> dcId_priority;
															#line 337 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 338 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					dcId_priority[it.dcId] = it.priority;
															#line 1768 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
															#line 340 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : oldConfig.regions ) {
															#line 341 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!dcId_priority.count(it.dcId) || dcId_priority[it.dcId] != it.priority)
															#line 1774 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 342 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGIONS_CHANGED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1778 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGIONS_CHANGED);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
															#line 347 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				int activeRegionCount = 0;
															#line 348 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				for( auto& it : newConfig.regions ) {
															#line 349 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (it.priority >= 0)
															#line 1791 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 350 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						activeRegionCount++;
															#line 1795 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					}
				}
															#line 353 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (activeRegionCount > 1)
															#line 1800 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 354 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1804 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::MULTIPLE_ACTIVE_REGIONS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
			}
															#line 358 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			fServerListF = tr->getRange(serverListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 360 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			fServerList = (newConfig.regions.size()) ? safeThreadFutureToFuture(fServerListF) : Future<RangeResult>();
															#line 363 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.usableRegions == 2)
															#line 1817 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 364 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (oldReplicationUsesDcId)
															#line 1821 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 365 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					fLocalityListF = tr->getRange(tagLocalityListKeys, CLIENT_KNOBS->TOO_MANY);
															#line 367 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					fLocalityList = safeThreadFutureToFuture(fLocalityListF);
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_1 = success(fLocalityList) || tooLong;
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont3when1(__when_expr_1.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 2;
															#line 368 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 1, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1836 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
				else
				{
															#line 390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					replicasFuturesF = std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>>();
															#line 392 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					replicasFutures = std::vector<Future<Optional<Value>>>();
															#line 393 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					for( auto& it : newConfig.regions ) {
															#line 394 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (it.priority >= 0)
															#line 1849 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						{
															#line 395 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
							replicasFuturesF.push_back(tr->get(datacenterReplicasKeyFor(it.dcId)));
															#line 396 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
							replicasFutures.push_back(safeThreadFutureToFuture(replicasFuturesF.back()));
															#line 1855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						}
					}
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					StrictFuture<Void> __when_expr_2 = waitForAll(replicasFutures) || tooLong;
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1862 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont3when2(__when_expr_2.get(), loopDepth); };
					static_cast<ChangeConfigActor*>(this)->actor_wait_state = 3;
															#line 399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 2, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1867 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					loopDepth = 0;
				}
			}
			else
			{
				loopDepth = a_body1loopBody1cont6(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1loopBody1cont4(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont4(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 412 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 1956 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 414 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = success(fServerList) || tooLong;
															#line 414 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 1962 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1loopBody1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_3.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 4;
															#line 414 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 3, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 1967 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1loopBody1cont22(loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont17(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont18(Void const& _,int loopDepth) 
	{
															#line 369 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fLocalityList.isReady())
															#line 1987 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 370 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 1991 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 372 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		RangeResult localityList = fLocalityList.get();
															#line 373 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!localityList.more && localityList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 375 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> localityDcIds;
															#line 376 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : localityList ) {
															#line 377 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto dc = decodeTagLocalityListKey(s.key);
															#line 378 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (dc.present())
															#line 2009 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 379 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				localityDcIds.insert(dc.get());
															#line 2013 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 383 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 384 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (localityDcIds.count(it.dcId) == 0)
															#line 2020 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 385 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DCID_MISSING); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2024 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DCID_MISSING);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont18(Void && _,int loopDepth) 
	{
															#line 369 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fLocalityList.isReady())
															#line 2039 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 370 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2043 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 372 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		RangeResult localityList = fLocalityList.get();
															#line 373 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!localityList.more && localityList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 375 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> localityDcIds;
															#line 376 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : localityList ) {
															#line 377 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto dc = decodeTagLocalityListKey(s.key);
															#line 378 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (dc.present())
															#line 2061 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 379 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				localityDcIds.insert(dc.get());
															#line 2065 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 383 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 384 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (localityDcIds.count(it.dcId) == 0)
															#line 2072 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 385 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DCID_MISSING); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2076 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DCID_MISSING);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont18(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont18(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont19(Void const& _,int loopDepth) 
	{
															#line 401 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : replicasFutures ) {
															#line 402 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!it.isReady())
															#line 2156 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 403 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2160 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 405 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!it.get().present())
															#line 2168 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 406 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_NOT_FULLY_REPLICATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2172 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_NOT_FULLY_REPLICATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont19(Void && _,int loopDepth) 
	{
															#line 401 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : replicasFutures ) {
															#line 402 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!it.isReady())
															#line 2189 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 403 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2193 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 405 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!it.get().present())
															#line 2201 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 406 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::REGION_NOT_FULLY_REPLICATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2205 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::REGION_NOT_FULLY_REPLICATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
		loopDepth = a_body1loopBody1cont17(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont19(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when2(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont3when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont22(int loopDepth) 
	{
															#line 437 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_4 = success(fWorkers) || tooLong;
															#line 437 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2285 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1loopBody1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1loopBody1cont22when1(__when_expr_4.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 5;
															#line 437 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 4, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2290 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont23(Void const& _,int loopDepth) 
	{
															#line 415 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fServerList.isReady())
															#line 2299 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 416 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2303 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 418 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		RangeResult serverList = fServerList.get();
															#line 419 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 421 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> newDcIds;
															#line 422 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 423 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			newDcIds.insert(it.dcId);
															#line 2319 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 425 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Optional<Key>> missingDcIds;
															#line 426 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : serverList ) {
															#line 427 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto ssi = decodeServerListValue(s.value);
															#line 428 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!ssi.locality.dcId().present() || !newDcIds.count(ssi.locality.dcId().get()))
															#line 2329 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 429 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				missingDcIds.insert(ssi.locality.dcId());
															#line 2333 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 432 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (missingDcIds.size() > (oldReplicationUsesDcId ? 1 : 0))
															#line 2338 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 433 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2342 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont23(Void && _,int loopDepth) 
	{
															#line 415 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fServerList.isReady())
															#line 2356 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 416 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2360 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 418 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		RangeResult serverList = fServerList.get();
															#line 419 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		ASSERT(!serverList.more && serverList.size() < CLIENT_KNOBS->TOO_MANY);
															#line 421 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Key> newDcIds;
															#line 422 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : newConfig.regions ) {
															#line 423 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			newDcIds.insert(it.dcId);
															#line 2376 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 425 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::set<Optional<Key>> missingDcIds;
															#line 426 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& s : serverList ) {
															#line 427 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto ssi = decodeServerListValue(s.value);
															#line 428 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!ssi.locality.dcId().present() || !newDcIds.count(ssi.locality.dcId().get()))
															#line 2386 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 429 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				missingDcIds.insert(ssi.locality.dcId());
															#line 2390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 432 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (missingDcIds.size() > (oldReplicationUsesDcId ? 1 : 0))
															#line 2395 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 433 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2399 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_IN_UNKNOWN_DCID);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		loopDepth = a_body1loopBody1cont22(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont23(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont23(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1loopBody1cont22cont1(Void const& _,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fWorkers.isReady())
															#line 2476 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 439 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2480 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 442 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 2488 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 443 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::map<Optional<Key>, std::set<Optional<Key>>> dcId_zoneIds;
															#line 444 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 445 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2496 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 446 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					dcId_zoneIds[it.locality.dcId()].insert(it.locality.zoneId());
															#line 2500 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 449 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& region : newConfig.regions ) {
															#line 450 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (dcId_zoneIds[region.dcId].size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2507 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 452 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2511 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 454 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (region.satelliteTLogReplicationFactor > 0 && region.priority >= 0)
															#line 2519 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 455 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					int totalSatelliteProcesses = 0;
															#line 456 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					for( auto& sat : region.satellites ) {
															#line 457 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						totalSatelliteProcesses += dcId_zoneIds[sat.dcId].size();
															#line 2527 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					}
															#line 459 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (totalSatelliteProcesses < region.satelliteTLogReplicationFactor)
															#line 2531 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 460 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2535 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
			}
		}
		else
		{
															#line 465 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::set<Optional<Key>> zoneIds;
															#line 466 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 467 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2552 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 468 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					zoneIds.insert(it.locality.zoneId());
															#line 2556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 471 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (zoneIds.size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2561 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 472 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2565 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 476 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageMigrationType == StorageMigrationType::DISABLED)
															#line 2574 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 478 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_MIGRATION_DISABLED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2578 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_MIGRATION_DISABLED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 479 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.storageMigrationType == StorageMigrationType::GRADUAL && newConfig.perpetualStorageWiggleSpeed == 0)
															#line 2588 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 481 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				warnPPWGradual = true;
															#line 2592 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 482 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_ROCKSDB_V1)
															#line 2598 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 484 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					warnRocksDBIsExperimental = true;
															#line 2602 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
		}
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22cont1(Void && _,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!fWorkers.isReady())
															#line 2614 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 439 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_UNAVAILABLE); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2618 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_UNAVAILABLE);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 442 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.regions.size())
															#line 2626 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 443 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::map<Optional<Key>, std::set<Optional<Key>>> dcId_zoneIds;
															#line 444 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 445 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2634 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 446 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					dcId_zoneIds[it.locality.dcId()].insert(it.locality.zoneId());
															#line 2638 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 449 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& region : newConfig.regions ) {
															#line 450 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (dcId_zoneIds[region.dcId].size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2645 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 452 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2649 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
					this->~ChangeConfigActorState();
					static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
					return 0;
				}
															#line 454 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (region.satelliteTLogReplicationFactor > 0 && region.priority >= 0)
															#line 2657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 455 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					int totalSatelliteProcesses = 0;
															#line 456 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					for( auto& sat : region.satellites ) {
															#line 457 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						totalSatelliteProcesses += dcId_zoneIds[sat.dcId].size();
															#line 2665 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					}
															#line 459 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					if (totalSatelliteProcesses < region.satelliteTLogReplicationFactor)
															#line 2669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
					{
															#line 460 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
						if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2673 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
						new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
						this->~ChangeConfigActorState();
						static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
						return 0;
					}
				}
			}
		}
		else
		{
															#line 465 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::set<Optional<Key>> zoneIds;
															#line 466 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& it : fWorkers.get() ) {
															#line 467 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (it.processClass.machineClassFitness(ProcessClass::Storage) <= ProcessClass::WorstFit)
															#line 2690 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 468 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					zoneIds.insert(it.locality.zoneId());
															#line 2694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
															#line 471 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (zoneIds.size() < std::max(newConfig.storageTeamSize, newConfig.tLogReplicationFactor))
															#line 2699 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 472 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::NOT_ENOUGH_WORKERS); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2703 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::NOT_ENOUGH_WORKERS);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}
															#line 476 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageMigrationType == StorageMigrationType::DISABLED)
															#line 2712 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 478 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::STORAGE_MIGRATION_DISABLED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2716 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::STORAGE_MIGRATION_DISABLED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 479 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (newConfig.storageMigrationType == StorageMigrationType::GRADUAL && newConfig.perpetualStorageWiggleSpeed == 0)
															#line 2726 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 481 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				warnPPWGradual = true;
															#line 2730 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 482 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (newConfig.storageServerStoreType != oldConfig.storageServerStoreType && newConfig.storageServerStoreType == KeyValueStoreType::SSD_ROCKSDB_V1)
															#line 2736 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				{
															#line 484 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
					warnRocksDBIsExperimental = true;
															#line 2740 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				}
			}
		}
		loopDepth = a_body1loopBody1cont4(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont22when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont22cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 4, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 4, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont22when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 4, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1cont22when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 4, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont24(Void const& _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont24(Void && _,int loopDepth) 
	{
		return a_body1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont24(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose6() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 5, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 5, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 5, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 5, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose6();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 5);

	}
	int a_body1loopBody1Catch1cont1(int loopDepth) 
	{
															#line 543 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_8 = safeThreadFutureToFuture(tr->onError(e1));
															#line 543 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2892 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_8.isReady()) { if (__when_expr_8.isError()) return a_body1Catch1(__when_expr_8.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1cont1when1(__when_expr_8.get(), loopDepth); };
		static_cast<ChangeConfigActor*>(this)->actor_wait_state = 9;
															#line 543 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_8.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 8, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2897 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1Catch1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1(int loopDepth) 
	{
		try {
															#line 524 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 525 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 526 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 527 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 529 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			vF = tr->get(initIdKey);
															#line 530 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<Value>> __when_expr_6 = safeThreadFutureToFuture(vF);
															#line 530 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 2926 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1loopBody1Catch1loopBody1Catch1(__when_expr_6.getError(), loopDepth); else return a_body1loopBody1Catch1loopBody1when1(__when_expr_6.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 7;
															#line 530 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 6, Optional<Value> >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2931 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1Catch1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1(const Error& e2,int loopDepth=0) 
	{
		try {
															#line 539 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_7 = safeThreadFutureToFuture(tr->onError(e2));
															#line 539 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ChangeConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2955 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_7.isReady()) { if (__when_expr_7.isError()) return a_body1Catch1(__when_expr_7.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1Catch1loopBody1Catch1when1(__when_expr_7.get(), loopDepth); };
			static_cast<ChangeConfigActor*>(this)->actor_wait_state = 8;
															#line 539 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_7.addCallbackAndClear(static_cast<ActorCallback< ChangeConfigActor, 7, Void >*>(static_cast<ChangeConfigActor*>(this)));
															#line 2960 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 2));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 2));
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont2(Optional<Value> const& v,int loopDepth) 
	{
															#line 531 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (v != m[initIdKey.toString()])
															#line 2975 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 532 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_ALREADY_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2979 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_ALREADY_CREATED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 533 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_ROCKSDB_V1))
															#line 2989 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 535 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 2993 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 537 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3003 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1cont2(Optional<Value> && v,int loopDepth) 
	{
															#line 531 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (v != m[initIdKey.toString()])
															#line 3017 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 532 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_ALREADY_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3021 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_ALREADY_CREATED);
			this->~ChangeConfigActorState();
			static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		else
		{
															#line 533 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (m[configKeysPrefix.toString() + "storage_engine"] == std::to_string(KeyValueStoreType::SSD_ROCKSDB_V1))
															#line 3031 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 535 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3035 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED_WARN_ROCKSDB_EXPERIMENTAL);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
			else
			{
															#line 537 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<ChangeConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::DATABASE_CREATED); this->~ChangeConfigActorState(); static_cast<ChangeConfigActor*>(this)->destroy(); return 0; }
															#line 3045 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<ChangeConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::DATABASE_CREATED);
				this->~ChangeConfigActorState();
				static_cast<ChangeConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(Optional<Value> const& v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont2(v, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1when1(Optional<Value> && v,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont2(std::move(v), loopDepth);

		return loopDepth;
	}
	void a_exitChoose7() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 6, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 6, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 6, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 6, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose7();
		try {
			a_body1loopBody1Catch1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 6);

	}
	int a_body1loopBody1Catch1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose8() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 7, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 7, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1loopBody1Catch1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 7, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1loopBody1Catch1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 7, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 7);
		a_exitChoose8();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 7);

	}
	int a_body1loopBody1Catch1cont3(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont3(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose9() 
	{
		if (static_cast<ChangeConfigActor*>(this)->actor_wait_state > 0) static_cast<ChangeConfigActor*>(this)->actor_wait_state = 0;
		static_cast<ChangeConfigActor*>(this)->ActorCallback< ChangeConfigActor, 8, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 8, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1loopBody1Catch1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_fire(ActorCallback< ChangeConfigActor, 8, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1loopBody1Catch1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
	void a_callback_error(ActorCallback< ChangeConfigActor, 8, Void >*,Error err) 
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), 8);
		a_exitChoose9();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), 8);

	}
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	std::map<std::string, std::string> m;
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool force;
															#line 262 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	StringRef initIdKey;
															#line 263 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 271 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool creating;
															#line 272 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Optional<UID> locked;
															#line 290 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<Void> tooLong;
															#line 291 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Key versionKey;
															#line 292 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool oldReplicationUsesDcId;
															#line 293 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool warnPPWGradual;
															#line 294 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool warnRocksDBIsExperimental;
															#line 303 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fConfigF;
															#line 305 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fConfig;
															#line 306 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processClassesF;
															#line 307 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processDataF;
															#line 308 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<std::vector<ProcessData>> fWorkers;
															#line 317 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DatabaseConfiguration oldConfig;
															#line 319 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DatabaseConfiguration newConfig;
															#line 358 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fServerListF;
															#line 360 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fServerList;
															#line 365 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> fLocalityListF;
															#line 367 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<RangeResult> fLocalityList;
															#line 390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	std::vector<typename DB::TransactionT::template FutureT<Optional<Value>>> replicasFuturesF;
															#line 392 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	std::vector<Future<Optional<Value>>> replicasFutures;
															#line 517 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Error e1;
															#line 529 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<Optional<Value>> vF;
															#line 3322 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via changeConfig()
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ChangeConfigActor final : public Actor<ConfigurationResult>, public ActorCallback< ChangeConfigActor<DB>, 0, Void >, public ActorCallback< ChangeConfigActor<DB>, 1, Void >, public ActorCallback< ChangeConfigActor<DB>, 2, Void >, public ActorCallback< ChangeConfigActor<DB>, 3, Void >, public ActorCallback< ChangeConfigActor<DB>, 4, Void >, public ActorCallback< ChangeConfigActor<DB>, 5, Void >, public ActorCallback< ChangeConfigActor<DB>, 6, Optional<Value> >, public ActorCallback< ChangeConfigActor<DB>, 7, Void >, public ActorCallback< ChangeConfigActor<DB>, 8, Void >, public FastAllocated<ChangeConfigActor<DB>>, public ChangeConfigActorState<DB, ChangeConfigActor<DB>> {
															#line 3329 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ChangeConfigActor<DB>>::operator new;
	using FastAllocated<ChangeConfigActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigurationResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ChangeConfigActor<DB>, 0, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 1, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 2, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 3, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 4, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 5, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 6, Optional<Value> >;
friend struct ActorCallback< ChangeConfigActor<DB>, 7, Void >;
friend struct ActorCallback< ChangeConfigActor<DB>, 8, Void >;
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ChangeConfigActor(Reference<DB> const& db,std::map<std::string, std::string> const& m,bool const& force) 
															#line 3348 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<ConfigurationResult>(),
		   ChangeConfigActorState<DB, ChangeConfigActor<DB>>(db, m, force)
	{
		fdb_probe_actor_enter("changeConfig", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("changeConfig");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("changeConfig", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 4, Void >*)0, actor_cancelled()); break;
		case 6: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 5, Void >*)0, actor_cancelled()); break;
		case 7: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 6, Optional<Value> >*)0, actor_cancelled()); break;
		case 8: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 7, Void >*)0, actor_cancelled()); break;
		case 9: this->a_callback_error((ActorCallback< ChangeConfigActor<DB>, 8, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<ConfigurationResult> changeConfig( Reference<DB> const& db, std::map<std::string, std::string> const& m, bool const& force ) {
															#line 260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<ConfigurationResult>(new ChangeConfigActor<DB>(db, m, force));
															#line 3385 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 555 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 3390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via autoConfig()
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class AutoConfigActor>
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class AutoConfigActorState {
															#line 3396 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	AutoConfigActorState(Reference<DB> const& db,ConfigureAutoResult const& conf) 
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   conf(conf),
															#line 558 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 559 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   versionKey(BinaryWriter::toValue(deterministicRandom()->randomUniqueID(), Unversioned()))
															#line 3409 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("autoConfig", reinterpret_cast<unsigned long>(this));

	}
	~AutoConfigActorState() 
	{
		fdb_probe_actor_destroy("autoConfig", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 561 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!conf.address_class.size())
															#line 3424 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 562 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::INCOMPLETE_CONFIGURATION); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3428 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::INCOMPLETE_CONFIGURATION);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 564 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 3436 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 566 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 567 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::PRIORITY_SYSTEM_IMMEDIATE);
															#line 568 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 569 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::USE_PROVISIONAL_PROXIES);
															#line 571 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processClassesF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 572 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			processDataF = typename DB::TransactionT::template FutureT<RangeResult>();
															#line 573 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<std::vector<ProcessData>> __when_expr_0 = getWorkers(tr, processClassesF, processDataF);
															#line 573 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3481 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<AutoConfigActor*>(this)->actor_wait_state = 1;
															#line 573 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*>(static_cast<AutoConfigActor*>(this)));
															#line 3486 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 620 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->onError(e));
															#line 620 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 3510 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_2.get(), loopDepth); };
			static_cast<AutoConfigActor*>(this)->actor_wait_state = 3;
															#line 620 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 2, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 3515 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> const& workers,int loopDepth) 
	{
															#line 574 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<NetworkAddress, Optional<Standalone<StringRef>>> address_processId;
															#line 575 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& w : workers ) {
															#line 576 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			address_processId[w.address] = w.locality.processId();
															#line 3534 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 579 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : conf.address_class ) {
															#line 580 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (it.second.classSource() == ProcessClass::CommandLineSource)
															#line 3540 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 581 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->clear(processClassKeyFor(address_processId[it.first].get()));
															#line 3544 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 583 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(processClassKeyFor(address_processId[it.first].get()), processClassValue(it.second));
															#line 3550 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 587 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.address_class.size())
															#line 3555 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 588 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 3559 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 590 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_logs != conf.old_logs)
															#line 3563 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 591 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_logs", format("%d", conf.auto_logs));
															#line 3567 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 593 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_commit_proxies != conf.old_commit_proxies)
															#line 3571 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 594 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_commit_proxies", format("%d", conf.auto_commit_proxies));
															#line 3575 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 596 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_grv_proxies != conf.old_grv_proxies)
															#line 3579 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 597 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_grv_proxies", format("%d", conf.auto_grv_proxies));
															#line 3583 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 599 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_resolvers != conf.old_resolvers)
															#line 3587 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 600 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_resolvers", format("%d", conf.auto_resolvers));
															#line 3591 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 602 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_replication != conf.old_replication)
															#line 3595 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 603 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<StringRef> modes;
															#line 604 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			modes.push_back(conf.auto_replication);
															#line 605 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::map<std::string, std::string> m;
															#line 606 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto r = buildConfiguration(modes, m);
															#line 607 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (r != ConfigurationResult::SUCCESS)
															#line 3607 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 608 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(r); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3611 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(r);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 610 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& kv : m ) {
															#line 611 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(kv.first, kv.second);
															#line 3621 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 614 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 615 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3632 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<AutoConfigActor*>(this)->actor_wait_state = 2;
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 1, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 3637 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::vector<ProcessData> && workers,int loopDepth) 
	{
															#line 574 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<NetworkAddress, Optional<Standalone<StringRef>>> address_processId;
															#line 575 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& w : workers ) {
															#line 576 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			address_processId[w.address] = w.locality.processId();
															#line 3650 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 579 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto& it : conf.address_class ) {
															#line 580 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (it.second.classSource() == ProcessClass::CommandLineSource)
															#line 3656 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 581 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->clear(processClassKeyFor(address_processId[it.first].get()));
															#line 3660 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
			else
			{
															#line 583 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(processClassKeyFor(address_processId[it.first].get()), processClassValue(it.second));
															#line 3666 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 587 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.address_class.size())
															#line 3671 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 588 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(processClassChangeKey, deterministicRandom()->randomUniqueID().toString());
															#line 3675 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 590 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_logs != conf.old_logs)
															#line 3679 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 591 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_logs", format("%d", conf.auto_logs));
															#line 3683 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 593 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_commit_proxies != conf.old_commit_proxies)
															#line 3687 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 594 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_commit_proxies", format("%d", conf.auto_commit_proxies));
															#line 3691 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 596 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_grv_proxies != conf.old_grv_proxies)
															#line 3695 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 597 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_grv_proxies", format("%d", conf.auto_grv_proxies));
															#line 3699 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 599 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_resolvers != conf.old_resolvers)
															#line 3703 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 600 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->set(configKeysPrefix.toString() + "auto_resolvers", format("%d", conf.auto_resolvers));
															#line 3707 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 602 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (conf.auto_replication != conf.old_replication)
															#line 3711 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 603 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::vector<StringRef> modes;
															#line 604 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			modes.push_back(conf.auto_replication);
															#line 605 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			std::map<std::string, std::string> m;
															#line 606 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			auto r = buildConfiguration(modes, m);
															#line 607 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (r != ConfigurationResult::SUCCESS)
															#line 3723 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 608 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(r); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3727 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(r);
				this->~AutoConfigActorState();
				static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
				return 0;
			}
															#line 610 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			for( auto& kv : m ) {
															#line 611 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				tr->set(kv.first, kv.second);
															#line 3737 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
		}
															#line 614 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->addReadConflictRange(singleKeyRange(moveKeysLockOwnerKey));
															#line 615 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(moveKeysLockOwnerKey, versionKey);
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->commit());
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 3748 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<AutoConfigActor*>(this)->actor_wait_state = 2;
															#line 617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AutoConfigActor, 1, Void >*>(static_cast<AutoConfigActor*>(this)));
															#line 3753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ProcessData> const& workers,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(workers, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::vector<ProcessData> && workers,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(workers), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,std::vector<ProcessData> && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 0, std::vector<ProcessData> >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont3(Void const& _,int loopDepth) 
	{
															#line 618 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3825 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Void && _,int loopDepth) 
	{
															#line 618 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<AutoConfigActor*>(this)->SAV<ConfigurationResult>::futures) { (void)(ConfigurationResult::SUCCESS); this->~AutoConfigActorState(); static_cast<AutoConfigActor*>(this)->destroy(); return 0; }
															#line 3837 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<AutoConfigActor*>(this)->SAV< ConfigurationResult >::value()) ConfigurationResult(ConfigurationResult::SUCCESS);
		this->~AutoConfigActorState();
		static_cast<AutoConfigActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<AutoConfigActor*>(this)->actor_wait_state > 0) static_cast<AutoConfigActor*>(this)->actor_wait_state = 0;
		static_cast<AutoConfigActor*>(this)->ActorCallback< AutoConfigActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< AutoConfigActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< AutoConfigActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ConfigureAutoResult conf;
															#line 558 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 559 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Key versionKey;
															#line 571 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processClassesF;
															#line 572 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename DB::TransactionT::template FutureT<RangeResult> processDataF;
															#line 3995 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via autoConfig()
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class AutoConfigActor final : public Actor<ConfigurationResult>, public ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >, public ActorCallback< AutoConfigActor<DB>, 1, Void >, public ActorCallback< AutoConfigActor<DB>, 2, Void >, public FastAllocated<AutoConfigActor<DB>>, public AutoConfigActorState<DB, AutoConfigActor<DB>> {
															#line 4002 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<AutoConfigActor<DB>>::operator new;
	using FastAllocated<AutoConfigActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<ConfigurationResult>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >;
friend struct ActorCallback< AutoConfigActor<DB>, 1, Void >;
friend struct ActorCallback< AutoConfigActor<DB>, 2, Void >;
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	AutoConfigActor(Reference<DB> const& db,ConfigureAutoResult const& conf) 
															#line 4015 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<ConfigurationResult>(),
		   AutoConfigActorState<DB, AutoConfigActor<DB>>(db, conf)
	{
		fdb_probe_actor_enter("autoConfig", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("autoConfig");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("autoConfig", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 0, std::vector<ProcessData> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< AutoConfigActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<ConfigurationResult> autoConfig( Reference<DB> const& db, ConfigureAutoResult const& conf ) {
															#line 556 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<ConfigurationResult>(new AutoConfigActor<DB>(db, conf));
															#line 4046 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 624 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

// Accepts tokens separated by spaces in a single string
template <class DB>
Future<ConfigurationResult> changeConfig(Reference<DB> db, std::string const& modes, bool force) {
	TraceEvent("ChangeConfig").detail("Mode", modes);
	std::map<std::string, std::string> m;
	auto r = buildConfiguration(modes, m);
	if (r != ConfigurationResult::SUCCESS)
		return r;
	return changeConfig(db, m, force);
}

// Accepts a vector of configuration tokens
template <class DB>
Future<ConfigurationResult> changeConfig(Reference<DB> db,
                                         std::vector<StringRef> const& modes,
                                         Optional<ConfigureAutoResult> const& conf,
                                         bool force) {
	if (modes.size() && modes[0] == LiteralStringRef("auto") && conf.present()) {
		return autoConfig(db, conf.get());
	}

	std::map<std::string, std::string> m;
	auto r = buildConfiguration(modes, m);
	if (r != ConfigurationResult::SUCCESS)
		return r;
	return changeConfig(db, m, force);
}

// return the corresponding error message for the CoordinatorsResult
// used by special keys and fdbcli
std::string generateErrorMessage(const CoordinatorsResult& res);

															#line 4083 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via tryGetTenantTransaction()
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction, class TryGetTenantTransactionActor>
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class TryGetTenantTransactionActorState {
															#line 4089 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TryGetTenantTransactionActorState(Transaction const& tr,TenantName const& name) 
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 659 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tenantMapKey(name.withPrefix(tenantMapPrefix))
															#line 4100 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~TryGetTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 661 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 662 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 664 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenantFuture = tr->get(tenantMapKey);
															#line 665 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(tenantFuture);
															#line 665 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4123 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 665 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantTransactionActor, 0, Optional<Value> >*>(static_cast<TryGetTenantTransactionActor*>(this)));
															#line 4128 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& val,int loopDepth) 
	{
															#line 666 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(val.map<TenantMapEntry>([](Optional<Value> v) { return decodeTenantEntry(v.get()); })); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 4151 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(val.map<TenantMapEntry>([](Optional<Value> v) { return decodeTenantEntry(v.get()); }));
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && val,int loopDepth) 
	{
															#line 666 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<TryGetTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(val.map<TenantMapEntry>([](Optional<Value> v) { return decodeTenantEntry(v.get()); })); this->~TryGetTenantTransactionActorState(); static_cast<TryGetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 4163 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<TryGetTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(val.map<TenantMapEntry>([](Optional<Value> v) { return decodeTenantEntry(v.get()); }));
		this->~TryGetTenantTransactionActorState();
		static_cast<TryGetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& val,int loopDepth) 
	{
		loopDepth = a_body1cont1(val, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && val,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(val), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantTransactionActor*>(this)->ActorCallback< TryGetTenantTransactionActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetTenantTransactionActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetTenantTransactionActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Transaction tr;
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 659 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Key tenantMapKey;
															#line 664 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, Optional<Value>>::type tenantFuture;
															#line 4242 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via tryGetTenantTransaction()
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class TryGetTenantTransactionActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<Value> >, public FastAllocated<TryGetTenantTransactionActor<Transaction>>, public TryGetTenantTransactionActorState<Transaction, TryGetTenantTransactionActor<Transaction>> {
															#line 4249 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<TryGetTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<TryGetTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<Value> >;
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TryGetTenantTransactionActor(Transaction const& tr,TenantName const& name) 
															#line 4260 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   TryGetTenantTransactionActorState<Transaction, TryGetTenantTransactionActor<Transaction>>(tr, name)
	{
		fdb_probe_actor_enter("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetTenantTransactionActor<Transaction>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> tryGetTenantTransaction( Transaction const& tr, TenantName const& name ) {
															#line 657 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Optional<TenantMapEntry>>(new TryGetTenantTransactionActor<Transaction>(tr, name));
															#line 4289 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 668 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 4294 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via tryGetTenant()
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class TryGetTenantActor>
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class TryGetTenantActorState {
															#line 4300 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TryGetTenantActorState(Reference<DB> const& db,TenantName const& name) 
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 671 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction())
															#line 4311 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("tryGetTenant", reinterpret_cast<unsigned long>(this));

	}
	~TryGetTenantActorState() 
	{
		fdb_probe_actor_destroy("tryGetTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 673 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 4326 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 675 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 676 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, name);
															#line 676 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<TryGetTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 4361 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<TryGetTenantActor*>(this)->actor_wait_state = 1;
															#line 676 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<TryGetTenantActor*>(this)));
															#line 4366 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 679 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->onError(e));
															#line 679 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<TryGetTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 4390 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<TryGetTenantActor*>(this)->actor_wait_state = 2;
															#line 679 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TryGetTenantActor, 1, Void >*>(static_cast<TryGetTenantActor*>(this)));
															#line 4395 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 677 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<TryGetTenantActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantActorState(); static_cast<TryGetTenantActor*>(this)->destroy(); return 0; }
															#line 4410 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<TryGetTenantActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 677 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<TryGetTenantActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(entry); this->~TryGetTenantActorState(); static_cast<TryGetTenantActor*>(this)->destroy(); return 0; }
															#line 4422 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<TryGetTenantActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(entry);
		this->~TryGetTenantActorState();
		static_cast<TryGetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TryGetTenantActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantActor*>(this)->ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TryGetTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TryGetTenantActor*>(this)->actor_wait_state > 0) static_cast<TryGetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<TryGetTenantActor*>(this)->ActorCallback< TryGetTenantActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TryGetTenantActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TryGetTenantActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 671 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 4574 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via tryGetTenant()
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class TryGetTenantActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< TryGetTenantActor<DB>, 0, Optional<TenantMapEntry> >, public ActorCallback< TryGetTenantActor<DB>, 1, Void >, public FastAllocated<TryGetTenantActor<DB>>, public TryGetTenantActorState<DB, TryGetTenantActor<DB>> {
															#line 4581 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<TryGetTenantActor<DB>>::operator new;
	using FastAllocated<TryGetTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TryGetTenantActor<DB>, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< TryGetTenantActor<DB>, 1, Void >;
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TryGetTenantActor(Reference<DB> const& db,TenantName const& name) 
															#line 4593 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   TryGetTenantActorState<DB, TryGetTenantActor<DB>>(db, name)
	{
		fdb_probe_actor_enter("tryGetTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tryGetTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tryGetTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TryGetTenantActor<DB>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TryGetTenantActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> tryGetTenant( Reference<DB> const& db, TenantName const& name ) {
															#line 669 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Optional<TenantMapEntry>>(new TryGetTenantActor<DB>(db, name));
															#line 4623 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 683 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 4628 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via getTenantTransaction()
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction, class GetTenantTransactionActor>
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetTenantTransactionActorState {
															#line 4634 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetTenantTransactionActorState(Transaction const& tr,TenantName const& name) 
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name)
															#line 4643 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("getTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("getTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 686 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, name);
															#line 686 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<GetTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4660 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 686 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*>(static_cast<GetTenantTransactionActor*>(this)));
															#line 4665 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 687 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 4688 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 688 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 4692 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 691 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetTenantTransactionActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantTransactionActorState(); static_cast<GetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 4696 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetTenantTransactionActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 687 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 4708 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 688 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 4712 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 691 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetTenantTransactionActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantTransactionActorState(); static_cast<GetTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 4716 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetTenantTransactionActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantTransactionActorState();
		static_cast<GetTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(entry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<GetTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantTransactionActor*>(this)->ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Transaction tr;
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 4791 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via getTenantTransaction()
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetTenantTransactionActor final : public Actor<TenantMapEntry>, public ActorCallback< GetTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >, public FastAllocated<GetTenantTransactionActor<Transaction>>, public GetTenantTransactionActorState<Transaction, GetTenantTransactionActor<Transaction>> {
															#line 4798 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<GetTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<GetTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TenantMapEntry>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >;
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetTenantTransactionActor(Transaction const& tr,TenantName const& name) 
															#line 4809 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<TenantMapEntry>(),
		   GetTenantTransactionActorState<Transaction, GetTenantTransactionActor<Transaction>>(tr, name)
	{
		fdb_probe_actor_enter("getTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<TenantMapEntry> getTenantTransaction( Transaction const& tr, TenantName const& name ) {
															#line 684 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<TenantMapEntry>(new GetTenantTransactionActor<Transaction>(tr, name));
															#line 4838 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 693 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 4843 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via getTenant()
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class GetTenantActor>
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetTenantActorState {
															#line 4849 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetTenantActorState(Reference<DB> const& db,TenantName const& name) 
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name)
															#line 4858 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("getTenant", reinterpret_cast<unsigned long>(this));

	}
	~GetTenantActorState() 
	{
		fdb_probe_actor_destroy("getTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 696 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenant(db, name);
															#line 696 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<GetTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 4875 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<GetTenantActor*>(this)->actor_wait_state = 1;
															#line 696 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<GetTenantActor*>(this)));
															#line 4880 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 697 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 4903 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 698 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 4907 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 701 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetTenantActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantActorState(); static_cast<GetTenantActor*>(this)->destroy(); return 0; }
															#line 4911 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetTenantActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 697 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 4923 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 698 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_found(), loopDepth);
															#line 4927 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 701 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<GetTenantActor*>(this)->SAV<TenantMapEntry>::futures) { (void)(entry.get()); this->~GetTenantActorState(); static_cast<GetTenantActor*>(this)->destroy(); return 0; }
															#line 4931 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<GetTenantActor*>(this)->SAV< TenantMapEntry >::value()) TenantMapEntry(entry.get());
		this->~GetTenantActorState();
		static_cast<GetTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(entry, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<GetTenantActor*>(this)->actor_wait_state > 0) static_cast<GetTenantActor*>(this)->actor_wait_state = 0;
		static_cast<GetTenantActor*>(this)->ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< GetTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 5006 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via getTenant()
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class GetTenantActor final : public Actor<TenantMapEntry>, public ActorCallback< GetTenantActor<DB>, 0, Optional<TenantMapEntry> >, public FastAllocated<GetTenantActor<DB>>, public GetTenantActorState<DB, GetTenantActor<DB>> {
															#line 5013 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<GetTenantActor<DB>>::operator new;
	using FastAllocated<GetTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<TenantMapEntry>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< GetTenantActor<DB>, 0, Optional<TenantMapEntry> >;
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	GetTenantActor(Reference<DB> const& db,TenantName const& name) 
															#line 5024 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<TenantMapEntry>(),
		   GetTenantActorState<DB, GetTenantActor<DB>>(db, name)
	{
		fdb_probe_actor_enter("getTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("getTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("getTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< GetTenantActor<DB>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		}

	}
};
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<TenantMapEntry> getTenant( Reference<DB> const& db, TenantName const& name ) {
															#line 694 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<TenantMapEntry>(new GetTenantActor<DB>(db, name));
															#line 5053 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 703 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

// Creates a tenant with the given name. If the tenant already exists, an empty optional will be returned.
															#line 5059 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via createTenantTransaction()
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction, class CreateTenantTransactionActor>
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class CreateTenantTransactionActorState {
															#line 5065 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	CreateTenantTransactionActorState(Transaction const& tr,TenantNameRef const& name) 
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 707 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tenantMapKey(name.withPrefix(tenantMapPrefix))
															#line 5076 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("createTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~CreateTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("createTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 709 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (name.startsWith("\xff"_sr))
															#line 5091 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 710 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				return a_body1Catch1(invalid_tenant_name(), loopDepth);
															#line 5095 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			}
															#line 713 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 714 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 716 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenantEntryFuture = tryGetTenantTransaction(tr, name);
															#line 717 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenantDataPrefixFuture = tr->get(tenantDataPrefixKey);
															#line 719 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			lastIdFuture = tr->get(tenantLastIdKey);
															#line 720 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenantModeFuture = tr->get(configKeysPrefix.withSuffix("tenant_mode"_sr));
															#line 723 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<Value>> __when_expr_0 = safeThreadFutureToFuture(tenantModeFuture);
															#line 723 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5113 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 723 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 0, Optional<Value> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5118 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> const& tenantMode,int loopDepth) 
	{
															#line 725 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!tenantMode.present() || tenantMode.get() == StringRef(format("%d", TenantMode::DISABLED)))
															#line 5141 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 726 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenants_disabled(), loopDepth);
															#line 5145 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = tenantEntryFuture;
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5151 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5156 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Optional<Value> && tenantMode,int loopDepth) 
	{
															#line 725 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!tenantMode.present() || tenantMode.get() == StringRef(format("%d", TenantMode::DISABLED)))
															#line 5165 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 726 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenants_disabled(), loopDepth);
															#line 5169 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = tenantEntryFuture;
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5175 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 729 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5180 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<Value> const& tenantMode,int loopDepth) 
	{
		loopDepth = a_body1cont1(tenantMode, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<Value> && tenantMode,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(tenantMode), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 0, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 0, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 0, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 0, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont3(Optional<TenantMapEntry> const& tenantEntry,int loopDepth) 
	{
															#line 730 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (tenantEntry.present())
															#line 5252 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 731 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(Optional<TenantMapEntry>()); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 5256 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(Optional<TenantMapEntry>());
			this->~CreateTenantTransactionActorState();
			static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<Value>> __when_expr_2 = safeThreadFutureToFuture(lastIdFuture);
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5266 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5271 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3(Optional<TenantMapEntry> && tenantEntry,int loopDepth) 
	{
															#line 730 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (tenantEntry.present())
															#line 5280 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 731 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(Optional<TenantMapEntry>()); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 5284 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(Optional<TenantMapEntry>());
			this->~CreateTenantTransactionActorState();
			static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<Value>> __when_expr_2 = safeThreadFutureToFuture(lastIdFuture);
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5294 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont3when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 3;
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5299 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> const& tenantEntry,int loopDepth) 
	{
		loopDepth = a_body1cont3(tenantEntry, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Optional<TenantMapEntry> && tenantEntry,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(tenantEntry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont5(int loopDepth) 
	{
															#line 735 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<Value>> __when_expr_3 = safeThreadFutureToFuture(tenantDataPrefixFuture);
															#line 735 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5373 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), loopDepth); else return a_body1cont5when1(__when_expr_3.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 4;
															#line 735 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 3, Optional<Value> >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5378 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont3when1(Optional<Value> const& __lastIdVal,int loopDepth) 
	{
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		lastIdVal = __lastIdVal;
															#line 5387 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	int a_body1cont3when1(Optional<Value> && __lastIdVal,int loopDepth) 
	{
		lastIdVal = std::move(__lastIdVal);
		loopDepth = a_body1cont5(loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont3when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 2, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1cont7(Optional<Value> const& tenantDataPrefix,int loopDepth) 
	{
															#line 737 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (tenantDataPrefix.present() && tenantDataPrefix.get().size() + TenantMapEntry::ROOT_PREFIX_SIZE > CLIENT_KNOBS->TENANT_PREFIX_SIZE_LIMIT)
															#line 5454 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 739 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			TraceEvent(SevWarnAlways, "TenantPrefixTooLarge") .detail("TenantSubspace", tenantDataPrefix.get()) .detail("TenantSubspaceLength", tenantDataPrefix.get().size()) .detail("RootPrefixLength", TenantMapEntry::ROOT_PREFIX_SIZE) .detail("MaxTenantPrefixSize", CLIENT_KNOBS->TENANT_PREFIX_SIZE_LIMIT);
															#line 745 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(client_invalid_operation(), loopDepth);
															#line 5460 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 748 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		newTenant = TenantMapEntry(lastIdVal.present() ? TenantMapEntry::prefixToId(lastIdVal.get()) + 1 : 0, tenantDataPrefix.present() ? (KeyRef)tenantDataPrefix.get() : ""_sr);
															#line 751 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		prefixRangeFuture = tr->getRange(prefixRange(newTenant.prefix), 1);
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_4 = safeThreadFutureToFuture(prefixRangeFuture);
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5470 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 4, RangeResult >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5475 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont7(Optional<Value> && tenantDataPrefix,int loopDepth) 
	{
															#line 737 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (tenantDataPrefix.present() && tenantDataPrefix.get().size() + TenantMapEntry::ROOT_PREFIX_SIZE > CLIENT_KNOBS->TENANT_PREFIX_SIZE_LIMIT)
															#line 5484 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 739 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			TraceEvent(SevWarnAlways, "TenantPrefixTooLarge") .detail("TenantSubspace", tenantDataPrefix.get()) .detail("TenantSubspaceLength", tenantDataPrefix.get().size()) .detail("RootPrefixLength", TenantMapEntry::ROOT_PREFIX_SIZE) .detail("MaxTenantPrefixSize", CLIENT_KNOBS->TENANT_PREFIX_SIZE_LIMIT);
															#line 745 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(client_invalid_operation(), loopDepth);
															#line 5490 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 748 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		newTenant = TenantMapEntry(lastIdVal.present() ? TenantMapEntry::prefixToId(lastIdVal.get()) + 1 : 0, tenantDataPrefix.present() ? (KeyRef)tenantDataPrefix.get() : ""_sr);
															#line 751 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		prefixRangeFuture = tr->getRange(prefixRange(newTenant.prefix), 1);
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_4 = safeThreadFutureToFuture(prefixRangeFuture);
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 5500 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), loopDepth); else return a_body1cont7when1(__when_expr_4.get(), loopDepth); };
		static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 5;
															#line 753 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< CreateTenantTransactionActor, 4, RangeResult >*>(static_cast<CreateTenantTransactionActor*>(this)));
															#line 5505 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont5when1(Optional<Value> const& tenantDataPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont7(tenantDataPrefix, loopDepth);

		return loopDepth;
	}
	int a_body1cont5when1(Optional<Value> && tenantDataPrefix,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(tenantDataPrefix), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 3, Optional<Value> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 3, Optional<Value> >*,Optional<Value> const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 3, Optional<Value> >*,Optional<Value> && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1cont5when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 3, Optional<Value> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont8(RangeResult const& contents,int loopDepth) 
	{
															#line 754 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!contents.empty())
															#line 5577 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 755 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_prefix_allocator_conflict(), loopDepth);
															#line 5581 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 758 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(tenantLastIdKey, TenantMapEntry::idToPrefix(newTenant.id));
															#line 759 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(tenantMapKey, encodeTenantEntry(newTenant));
															#line 761 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(newTenant); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 5589 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(std::move(newTenant)); // state_var_RVO
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8(RangeResult && contents,int loopDepth) 
	{
															#line 754 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!contents.empty())
															#line 5601 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 755 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_prefix_allocator_conflict(), loopDepth);
															#line 5605 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 758 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(tenantLastIdKey, TenantMapEntry::idToPrefix(newTenant.id));
															#line 759 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->set(tenantMapKey, encodeTenantEntry(newTenant));
															#line 761 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<CreateTenantTransactionActor*>(this)->SAV<Optional<TenantMapEntry>>::futures) { (void)(newTenant); this->~CreateTenantTransactionActorState(); static_cast<CreateTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 5613 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<CreateTenantTransactionActor*>(this)->SAV< Optional<TenantMapEntry> >::value()) Optional<TenantMapEntry>(std::move(newTenant)); // state_var_RVO
		this->~CreateTenantTransactionActorState();
		static_cast<CreateTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7when1(RangeResult const& contents,int loopDepth) 
	{
		loopDepth = a_body1cont8(contents, loopDepth);

		return loopDepth;
	}
	int a_body1cont7when1(RangeResult && contents,int loopDepth) 
	{
		loopDepth = a_body1cont8(std::move(contents), loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantTransactionActor*>(this)->ActorCallback< CreateTenantTransactionActor, 4, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 4, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< CreateTenantTransactionActor, 4, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1cont7when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< CreateTenantTransactionActor, 4, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), 4);

	}
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Transaction tr;
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantNameRef name;
															#line 707 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Key tenantMapKey;
															#line 716 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Future<Optional<TenantMapEntry>> tenantEntryFuture;
															#line 717 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, Optional<Value>>::type tenantDataPrefixFuture;
															#line 719 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, Optional<Value>>::type lastIdFuture;
															#line 720 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, Optional<Value>>::type tenantModeFuture;
															#line 734 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Optional<Value> lastIdVal;
															#line 748 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantMapEntry newTenant;
															#line 751 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type prefixRangeFuture;
															#line 5704 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via createTenantTransaction()
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class CreateTenantTransactionActor final : public Actor<Optional<TenantMapEntry>>, public ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Optional<Value> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 2, Optional<Value> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 3, Optional<Value> >, public ActorCallback< CreateTenantTransactionActor<Transaction>, 4, RangeResult >, public FastAllocated<CreateTenantTransactionActor<Transaction>>, public CreateTenantTransactionActorState<Transaction, CreateTenantTransactionActor<Transaction>> {
															#line 5711 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<CreateTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<CreateTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Optional<Value> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 2, Optional<Value> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 3, Optional<Value> >;
friend struct ActorCallback< CreateTenantTransactionActor<Transaction>, 4, RangeResult >;
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	CreateTenantTransactionActor(Transaction const& tr,TenantNameRef const& name) 
															#line 5726 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Optional<TenantMapEntry>>(),
		   CreateTenantTransactionActorState<Transaction, CreateTenantTransactionActor<Transaction>>(tr, name)
	{
		fdb_probe_actor_enter("createTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 0, Optional<Value> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 2, Optional<Value> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 3, Optional<Value> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< CreateTenantTransactionActor<Transaction>, 4, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Optional<TenantMapEntry>> createTenantTransaction( Transaction const& tr, TenantNameRef const& name ) {
															#line 705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Optional<TenantMapEntry>>(new CreateTenantTransactionActor<Transaction>(tr, name));
															#line 5759 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 763 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 5764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via createTenant()
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class CreateTenantActor>
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class CreateTenantActorState {
															#line 5770 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	CreateTenantActorState(Reference<DB> const& db,TenantName const& name) 
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 766 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 768 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   firstTry(true)
															#line 5783 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("createTenant", reinterpret_cast<unsigned long>(this));

	}
	~CreateTenantActorState() 
	{
		fdb_probe_actor_destroy("createTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 769 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 5798 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 771 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 773 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (firstTry)
															#line 5831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 774 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, name);
															#line 774 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5837 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<CreateTenantActor*>(this)->actor_wait_state = 1;
															#line 774 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<CreateTenantActor*>(this)));
															#line 5842 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 802 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 802 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 5871 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<CreateTenantActor*>(this)->actor_wait_state = 4;
															#line 802 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 3, Void >*>(static_cast<CreateTenantActor*>(this)));
															#line 5876 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 782 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Optional<TenantMapEntry>> __when_expr_1 = createTenantTransaction(tr, name);
															#line 782 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 5893 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<CreateTenantActor*>(this)->actor_wait_state = 2;
															#line 782 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 1, Optional<TenantMapEntry> >*>(static_cast<CreateTenantActor*>(this)));
															#line 5898 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 775 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (entry.present())
															#line 5907 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 776 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 5911 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 779 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		firstTry = false;
															#line 5915 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 775 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (entry.present())
															#line 5924 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 776 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(tenant_already_exists(), loopDepth);
															#line 5928 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 779 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		firstTry = false;
															#line 5932 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(int loopDepth) 
	{
															#line 784 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 6004 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 785 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 6008 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 788 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 788 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6014 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<CreateTenantActor*>(this)->actor_wait_state = 3;
															#line 788 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< CreateTenantActor, 2, Void >*>(static_cast<CreateTenantActor*>(this)));
															#line 6019 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<TenantMapEntry> const& __newTenant,int loopDepth) 
	{
															#line 782 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		newTenant = __newTenant;
															#line 6028 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Optional<TenantMapEntry> && __newTenant,int loopDepth) 
	{
		newTenant = std::move(__newTenant);
		loopDepth = a_body1loopBody1cont6(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 1, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 1, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 1, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 790 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 6095 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 791 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 6099 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 794 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent("CreatedTenant") .detail("Tenant", name) .detail("TenantId", newTenant.present() ? newTenant.get().id : -1) .detail("Prefix", newTenant.present() ? (StringRef)newTenant.get().prefix : "Unknown"_sr) .detail("Version", tr->getCommittedVersion());
															#line 800 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<CreateTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTenantActorState(); static_cast<CreateTenantActor*>(this)->destroy(); return 0; }
															#line 6105 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<CreateTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 790 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 6117 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 791 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 6121 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 794 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent("CreatedTenant") .detail("Tenant", name) .detail("TenantId", newTenant.present() ? newTenant.get().id : -1) .detail("Prefix", newTenant.present() ? (StringRef)newTenant.get().prefix : "Unknown"_sr) .detail("Version", tr->getCommittedVersion());
															#line 800 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<CreateTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~CreateTenantActorState(); static_cast<CreateTenantActor*>(this)->destroy(); return 0; }
															#line 6127 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<CreateTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~CreateTenantActorState();
		static_cast<CreateTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<CreateTenantActor*>(this)->actor_wait_state > 0) static_cast<CreateTenantActor*>(this)->actor_wait_state = 0;
		static_cast<CreateTenantActor*>(this)->ActorCallback< CreateTenantActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< CreateTenantActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< CreateTenantActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 766 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 768 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool firstTry;
															#line 782 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Optional<TenantMapEntry> newTenant;
															#line 6283 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via createTenant()
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class CreateTenantActor final : public Actor<Void>, public ActorCallback< CreateTenantActor<DB>, 0, Optional<TenantMapEntry> >, public ActorCallback< CreateTenantActor<DB>, 1, Optional<TenantMapEntry> >, public ActorCallback< CreateTenantActor<DB>, 2, Void >, public ActorCallback< CreateTenantActor<DB>, 3, Void >, public FastAllocated<CreateTenantActor<DB>>, public CreateTenantActorState<DB, CreateTenantActor<DB>> {
															#line 6290 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<CreateTenantActor<DB>>::operator new;
	using FastAllocated<CreateTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< CreateTenantActor<DB>, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< CreateTenantActor<DB>, 1, Optional<TenantMapEntry> >;
friend struct ActorCallback< CreateTenantActor<DB>, 2, Void >;
friend struct ActorCallback< CreateTenantActor<DB>, 3, Void >;
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	CreateTenantActor(Reference<DB> const& db,TenantName const& name) 
															#line 6304 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Void>(),
		   CreateTenantActorState<DB, CreateTenantActor<DB>>(db, name)
	{
		fdb_probe_actor_enter("createTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("createTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("createTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 1, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< CreateTenantActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Void> createTenant( Reference<DB> const& db, TenantName const& name ) {
															#line 764 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Void>(new CreateTenantActor<DB>(db, name));
															#line 6336 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 806 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 6341 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via deleteTenantTransaction()
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction, class DeleteTenantTransactionActor>
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class DeleteTenantTransactionActorState {
															#line 6347 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DeleteTenantTransactionActorState(Transaction const& tr,TenantNameRef const& name) 
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 809 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tenantMapKey(name.withPrefix(tenantMapPrefix))
															#line 6358 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("deleteTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	~DeleteTenantTransactionActorState() 
	{
		fdb_probe_actor_destroy("deleteTenantTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 811 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 812 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::LOCK_AWARE);
															#line 814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, name);
															#line 814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6379 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 1;
															#line 814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 0, Optional<TenantMapEntry> >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 6384 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~DeleteTenantTransactionActorState();
		static_cast<DeleteTenantTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 815 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!tenantEntry.present())
															#line 6407 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 816 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (!static_cast<DeleteTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantTransactionActorState(); static_cast<DeleteTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 6411 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			new (&static_cast<DeleteTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~DeleteTenantTransactionActorState();
			static_cast<DeleteTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 819 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		prefixRangeFuture = tr->getRange(prefixRange(tenantEntry.get().prefix), 1);
															#line 821 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<RangeResult> __when_expr_1 = safeThreadFutureToFuture(prefixRangeFuture);
															#line 821 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 6423 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 2;
															#line 821 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantTransactionActor, 1, RangeResult >*>(static_cast<DeleteTenantTransactionActor*>(this)));
															#line 6428 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> const& __tenantEntry,int loopDepth) 
	{
															#line 814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tenantEntry = __tenantEntry;
															#line 6437 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<TenantMapEntry> && __tenantEntry,int loopDepth) 
	{
		tenantEntry = std::move(__tenantEntry);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(RangeResult const& contents,int loopDepth) 
	{
															#line 822 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!contents.empty())
															#line 6504 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 823 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_empty(), loopDepth);
															#line 6508 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 826 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->clear(tenantMapKey);
															#line 828 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<DeleteTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantTransactionActorState(); static_cast<DeleteTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 6514 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<DeleteTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantTransactionActorState();
		static_cast<DeleteTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(RangeResult && contents,int loopDepth) 
	{
															#line 822 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!contents.empty())
															#line 6526 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 823 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1Catch1(tenant_not_empty(), loopDepth);
															#line 6530 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 826 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		tr->clear(tenantMapKey);
															#line 828 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<DeleteTenantTransactionActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantTransactionActorState(); static_cast<DeleteTenantTransactionActor*>(this)->destroy(); return 0; }
															#line 6536 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<DeleteTenantTransactionActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantTransactionActorState();
		static_cast<DeleteTenantTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult const& contents,int loopDepth) 
	{
		loopDepth = a_body1cont2(contents, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(RangeResult && contents,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(contents), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantTransactionActor*>(this)->ActorCallback< DeleteTenantTransactionActor, 1, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 1, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteTenantTransactionActor, 1, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteTenantTransactionActor, 1, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Transaction tr;
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantNameRef name;
															#line 809 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Key tenantMapKey;
															#line 814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Optional<TenantMapEntry> tenantEntry;
															#line 819 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type prefixRangeFuture;
															#line 6617 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via deleteTenantTransaction()
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class DeleteTenantTransactionActor final : public Actor<Void>, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >, public ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, RangeResult >, public FastAllocated<DeleteTenantTransactionActor<Transaction>>, public DeleteTenantTransactionActorState<Transaction, DeleteTenantTransactionActor<Transaction>> {
															#line 6624 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<DeleteTenantTransactionActor<Transaction>>::operator new;
	using FastAllocated<DeleteTenantTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, RangeResult >;
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DeleteTenantTransactionActor(Transaction const& tr,TenantNameRef const& name) 
															#line 6636 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Void>(),
		   DeleteTenantTransactionActorState<Transaction, DeleteTenantTransactionActor<Transaction>>(tr, name)
	{
		fdb_probe_actor_enter("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteTenantTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteTenantTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantTransactionActor<Transaction>, 1, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Void> deleteTenantTransaction( Transaction const& tr, TenantNameRef const& name ) {
															#line 807 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Void>(new DeleteTenantTransactionActor<Transaction>(tr, name));
															#line 6666 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 830 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 6671 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via deleteTenant()
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class DeleteTenantActor>
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class DeleteTenantActorState {
															#line 6677 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DeleteTenantActorState(Reference<DB> const& db,TenantName const& name) 
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   name(name),
															#line 833 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction()),
															#line 835 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   firstTry(true)
															#line 6690 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("deleteTenant", reinterpret_cast<unsigned long>(this));

	}
	~DeleteTenantActorState() 
	{
		fdb_probe_actor_destroy("deleteTenant", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 836 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 6705 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 838 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::ACCESS_SYSTEM_KEYS);
															#line 840 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (firstTry)
															#line 6738 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			{
															#line 841 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				StrictFuture<Optional<TenantMapEntry>> __when_expr_0 = tryGetTenantTransaction(tr, name);
															#line 841 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6744 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<DeleteTenantActor*>(this)->actor_wait_state = 1;
															#line 841 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 0, Optional<TenantMapEntry> >*>(static_cast<DeleteTenantActor*>(this)));
															#line 6749 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont2(loopDepth);
			}
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 864 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_3 = safeThreadFutureToFuture(tr->onError(e));
															#line 864 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 6778 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_3.get(), loopDepth); };
			static_cast<DeleteTenantActor*>(this)->actor_wait_state = 4;
															#line 864 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 3, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 6783 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(int loopDepth) 
	{
															#line 849 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_1 = deleteTenantTransaction(tr, name);
															#line 849 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6800 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1loopBody1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1loopBody1cont2when1(__when_expr_1.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 2;
															#line 849 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 1, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 6805 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
															#line 842 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 6814 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 843 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 6818 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 846 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		firstTry = false;
															#line 6822 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont3(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
															#line 842 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!entry.present())
															#line 6831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 843 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(tenant_not_found(), loopDepth);
															#line 6835 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 846 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		firstTry = false;
															#line 6839 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = a_body1loopBody1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> const& entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(entry, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Optional<TenantMapEntry> && entry,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont3(std::move(entry), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 0, Optional<TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 0, Optional<TenantMapEntry> >*,Optional<TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 0, Optional<TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont6(Void const& _,int loopDepth) 
	{
															#line 851 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 6911 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 852 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 6915 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6921 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 3;
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 2, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 6926 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6(Void && _,int loopDepth) 
	{
															#line 851 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 6935 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 852 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 6939 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		StrictFuture<Void> __when_expr_2 = safeThreadFutureToFuture(tr->commit());
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 6945 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1loopBody1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1loopBody1cont6when1(__when_expr_2.get(), loopDepth); };
		static_cast<DeleteTenantActor*>(this)->actor_wait_state = 3;
															#line 855 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< DeleteTenantActor, 2, Void >*>(static_cast<DeleteTenantActor*>(this)));
															#line 6950 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont2when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont6(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 857 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 7022 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 858 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 7026 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 861 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent("DeletedTenant").detail("Tenant", name).detail("Version", tr->getCommittedVersion());
															#line 862 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<DeleteTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantActorState(); static_cast<DeleteTenantActor*>(this)->destroy(); return 0; }
															#line 7032 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<DeleteTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 857 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (BUGGIFY)
															#line 7044 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		{
															#line 858 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			return a_body1loopBody1Catch1(commit_unknown_result(), loopDepth);
															#line 7048 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 861 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		TraceEvent("DeletedTenant").detail("Tenant", name).detail("Version", tr->getCommittedVersion());
															#line 862 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<DeleteTenantActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~DeleteTenantActorState(); static_cast<DeleteTenantActor*>(this)->destroy(); return 0; }
															#line 7054 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<DeleteTenantActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~DeleteTenantActorState();
		static_cast<DeleteTenantActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont6when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont6when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<DeleteTenantActor*>(this)->actor_wait_state > 0) static_cast<DeleteTenantActor*>(this)->actor_wait_state = 0;
		static_cast<DeleteTenantActor*>(this)->ActorCallback< DeleteTenantActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< DeleteTenantActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< DeleteTenantActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), 3);

	}
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName name;
															#line 833 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 835 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	bool firstTry;
															#line 7208 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via deleteTenant()
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class DeleteTenantActor final : public Actor<Void>, public ActorCallback< DeleteTenantActor<DB>, 0, Optional<TenantMapEntry> >, public ActorCallback< DeleteTenantActor<DB>, 1, Void >, public ActorCallback< DeleteTenantActor<DB>, 2, Void >, public ActorCallback< DeleteTenantActor<DB>, 3, Void >, public FastAllocated<DeleteTenantActor<DB>>, public DeleteTenantActorState<DB, DeleteTenantActor<DB>> {
															#line 7215 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<DeleteTenantActor<DB>>::operator new;
	using FastAllocated<DeleteTenantActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< DeleteTenantActor<DB>, 0, Optional<TenantMapEntry> >;
friend struct ActorCallback< DeleteTenantActor<DB>, 1, Void >;
friend struct ActorCallback< DeleteTenantActor<DB>, 2, Void >;
friend struct ActorCallback< DeleteTenantActor<DB>, 3, Void >;
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	DeleteTenantActor(Reference<DB> const& db,TenantName const& name) 
															#line 7229 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<Void>(),
		   DeleteTenantActorState<DB, DeleteTenantActor<DB>>(db, name)
	{
		fdb_probe_actor_enter("deleteTenant", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("deleteTenant");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("deleteTenant", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 0, Optional<TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 2, Void >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< DeleteTenantActor<DB>, 3, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<Void> deleteTenant( Reference<DB> const& db, TenantName const& name ) {
															#line 831 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<Void>(new DeleteTenantActor<DB>(db, name));
															#line 7261 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 868 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 7266 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via listTenantsTransaction()
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction, class ListTenantsTransactionActor>
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ListTenantsTransactionActorState {
															#line 7272 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ListTenantsTransactionActorState(Transaction const& tr,TenantNameRef const& begin,TenantNameRef const& end,int const& limit) 
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : tr(tr),
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   begin(begin),
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   end(end),
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   limit(limit),
															#line 874 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   range(KeyRangeRef(begin, end).withPrefix(tenantMapPrefix))
															#line 7287 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("listTenantsTransaction", reinterpret_cast<unsigned long>(this));

	}
	~ListTenantsTransactionActorState() 
	{
		fdb_probe_actor_destroy("listTenantsTransaction", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 876 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::RAW_ACCESS);
															#line 877 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::READ_LOCK_AWARE);
															#line 879 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			listFuture = tr->getRange(firstGreaterOrEqual(range.begin), firstGreaterOrEqual(range.end), limit);
															#line 881 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<RangeResult> __when_expr_0 = safeThreadFutureToFuture(listFuture);
															#line 881 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ListTenantsTransactionActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 7310 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListTenantsTransactionActor*>(this)->actor_wait_state = 1;
															#line 881 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListTenantsTransactionActor, 0, RangeResult >*>(static_cast<ListTenantsTransactionActor*>(this)));
															#line 7315 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
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
		this->~ListTenantsTransactionActorState();
		static_cast<ListTenantsTransactionActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult const& results,int loopDepth) 
	{
															#line 883 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<TenantName, TenantMapEntry> tenants;
															#line 884 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto kv : results ) {
															#line 885 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenants[kv.key.removePrefix(tenantMapPrefix)] = decodeTenantEntry(kv.value);
															#line 7342 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 888 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ListTenantsTransactionActor*>(this)->SAV<std::map<TenantName, TenantMapEntry>>::futures) { (void)(tenants); this->~ListTenantsTransactionActorState(); static_cast<ListTenantsTransactionActor*>(this)->destroy(); return 0; }
															#line 7346 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ListTenantsTransactionActor*>(this)->SAV< std::map<TenantName, TenantMapEntry> >::value()) std::map<TenantName, TenantMapEntry>(tenants);
		this->~ListTenantsTransactionActorState();
		static_cast<ListTenantsTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1(RangeResult && results,int loopDepth) 
	{
															#line 883 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		std::map<TenantName, TenantMapEntry> tenants;
															#line 884 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		for( auto kv : results ) {
															#line 885 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tenants[kv.key.removePrefix(tenantMapPrefix)] = decodeTenantEntry(kv.value);
															#line 7362 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		}
															#line 888 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ListTenantsTransactionActor*>(this)->SAV<std::map<TenantName, TenantMapEntry>>::futures) { (void)(tenants); this->~ListTenantsTransactionActorState(); static_cast<ListTenantsTransactionActor*>(this)->destroy(); return 0; }
															#line 7366 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ListTenantsTransactionActor*>(this)->SAV< std::map<TenantName, TenantMapEntry> >::value()) std::map<TenantName, TenantMapEntry>(tenants);
		this->~ListTenantsTransactionActorState();
		static_cast<ListTenantsTransactionActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(RangeResult const& results,int loopDepth) 
	{
		loopDepth = a_body1cont1(results, loopDepth);

		return loopDepth;
	}
	int a_body1when1(RangeResult && results,int loopDepth) 
	{
		loopDepth = a_body1cont1(std::move(results), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListTenantsTransactionActor*>(this)->actor_wait_state > 0) static_cast<ListTenantsTransactionActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantsTransactionActor*>(this)->ActorCallback< ListTenantsTransactionActor, 0, RangeResult >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantsTransactionActor, 0, RangeResult >*,RangeResult const& value) 
	{
		fdb_probe_actor_enter("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListTenantsTransactionActor, 0, RangeResult >*,RangeResult && value) 
	{
		fdb_probe_actor_enter("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListTenantsTransactionActor, 0, RangeResult >*,Error err) 
	{
		fdb_probe_actor_enter("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenantsTransaction", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Transaction tr;
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantNameRef begin;
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantNameRef end;
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	int limit;
															#line 874 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	KeyRange range;
															#line 879 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	typename transaction_future_type<Transaction, RangeResult>::type listFuture;
															#line 7449 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via listTenantsTransaction()
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ListTenantsTransactionActor final : public Actor<std::map<TenantName, TenantMapEntry>>, public ActorCallback< ListTenantsTransactionActor<Transaction>, 0, RangeResult >, public FastAllocated<ListTenantsTransactionActor<Transaction>>, public ListTenantsTransactionActorState<Transaction, ListTenantsTransactionActor<Transaction>> {
															#line 7456 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ListTenantsTransactionActor<Transaction>>::operator new;
	using FastAllocated<ListTenantsTransactionActor<Transaction>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::map<TenantName, TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListTenantsTransactionActor<Transaction>, 0, RangeResult >;
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ListTenantsTransactionActor(Transaction const& tr,TenantNameRef const& begin,TenantNameRef const& end,int const& limit) 
															#line 7467 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<std::map<TenantName, TenantMapEntry>>(),
		   ListTenantsTransactionActorState<Transaction, ListTenantsTransactionActor<Transaction>>(tr, begin, end, limit)
	{
		fdb_probe_actor_enter("listTenantsTransaction", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listTenantsTransaction");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listTenantsTransaction", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListTenantsTransactionActor<Transaction>, 0, RangeResult >*)0, actor_cancelled()); break;
		}

	}
};
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class Transaction>
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<std::map<TenantName, TenantMapEntry>> listTenantsTransaction( Transaction const& tr, TenantNameRef const& begin, TenantNameRef const& end, int const& limit ) {
															#line 869 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<std::map<TenantName, TenantMapEntry>>(new ListTenantsTransactionActor<Transaction>(tr, begin, end, limit));
															#line 7496 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 890 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"

															#line 7501 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
// This generated class is to be used only via listTenants()
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB, class ListTenantsActor>
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ListTenantsActorState {
															#line 7507 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ListTenantsActorState(Reference<DB> const& db,TenantName const& begin,TenantName const& end,int const& limit) 
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		 : db(db),
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   begin(begin),
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   end(end),
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   limit(limit),
															#line 896 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		   tr(db->createTransaction())
															#line 7522 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
	{
		fdb_probe_actor_create("listTenants", reinterpret_cast<unsigned long>(this));

	}
	~ListTenantsActorState() 
	{
		fdb_probe_actor_destroy("listTenants", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 898 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			;
															#line 7537 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = a_body1loopHead1(loopDepth);
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
		this->~ListTenantsActorState();
		static_cast<ListTenantsActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1(int loopDepth) 
	{
		try {
															#line 900 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			tr->setOption(FDBTransactionOptions::READ_SYSTEM_KEYS);
															#line 901 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<std::map<TenantName, TenantMapEntry>> __when_expr_0 = listTenantsTransaction(tr, begin, end, limit);
															#line 901 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ListTenantsActor*>(this)->actor_wait_state < 0) return a_body1loopBody1Catch1(actor_cancelled(), loopDepth);
															#line 7572 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1loopBody1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ListTenantsActor*>(this)->actor_wait_state = 1;
															#line 901 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ListTenantsActor, 0, std::map<TenantName, TenantMapEntry> >*>(static_cast<ListTenantsActor*>(this)));
															#line 7577 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1loopBody1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1loopBody1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 904 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			StrictFuture<Void> __when_expr_1 = safeThreadFutureToFuture(tr->onError(e));
															#line 904 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			if (static_cast<ListTenantsActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 7601 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1Catch1when1(__when_expr_1.get(), loopDepth); };
			static_cast<ListTenantsActor*>(this)->actor_wait_state = 2;
															#line 904 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ListTenantsActor, 1, Void >*>(static_cast<ListTenantsActor*>(this)));
															#line 7606 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
			loopDepth = 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
															#line 902 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ListTenantsActor*>(this)->SAV<std::map<TenantName, TenantMapEntry>>::futures) { (void)(tenants); this->~ListTenantsActorState(); static_cast<ListTenantsActor*>(this)->destroy(); return 0; }
															#line 7621 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ListTenantsActor*>(this)->SAV< std::map<TenantName, TenantMapEntry> >::value()) std::map<TenantName, TenantMapEntry>(tenants);
		this->~ListTenantsActorState();
		static_cast<ListTenantsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1cont2(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
															#line 902 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
		if (!static_cast<ListTenantsActor*>(this)->SAV<std::map<TenantName, TenantMapEntry>>::futures) { (void)(tenants); this->~ListTenantsActorState(); static_cast<ListTenantsActor*>(this)->destroy(); return 0; }
															#line 7633 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		new (&static_cast<ListTenantsActor*>(this)->SAV< std::map<TenantName, TenantMapEntry> >::value()) std::map<TenantName, TenantMapEntry>(tenants);
		this->~ListTenantsActorState();
		static_cast<ListTenantsActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1loopBody1when1(std::map<TenantName, TenantMapEntry> const& tenants,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(tenants, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(std::map<TenantName, TenantMapEntry> && tenants,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont2(std::move(tenants), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ListTenantsActor*>(this)->actor_wait_state > 0) static_cast<ListTenantsActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantsActor*>(this)->ActorCallback< ListTenantsActor, 0, std::map<TenantName, TenantMapEntry> >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantsActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> const& value) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ListTenantsActor, 0, std::map<TenantName, TenantMapEntry> >*,std::map<TenantName, TenantMapEntry> && value) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ListTenantsActor, 0, std::map<TenantName, TenantMapEntry> >*,Error err) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1loopBody1Catch1(error, 0);
		} catch (...) {
			a_body1loopBody1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1Catch1cont1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1cont1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1Catch1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1Catch1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ListTenantsActor*>(this)->actor_wait_state > 0) static_cast<ListTenantsActor*>(this)->actor_wait_state = 0;
		static_cast<ListTenantsActor*>(this)->ActorCallback< ListTenantsActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ListTenantsActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ListTenantsActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1Catch1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ListTenantsActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<DB> db;
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName begin;
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	TenantName end;
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	int limit;
															#line 896 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	Reference<typename DB::TransactionT> tr;
															#line 7789 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
};
// This generated class is to be used only via listTenants()
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
class ListTenantsActor final : public Actor<std::map<TenantName, TenantMapEntry>>, public ActorCallback< ListTenantsActor<DB>, 0, std::map<TenantName, TenantMapEntry> >, public ActorCallback< ListTenantsActor<DB>, 1, Void >, public FastAllocated<ListTenantsActor<DB>>, public ListTenantsActorState<DB, ListTenantsActor<DB>> {
															#line 7796 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
public:
	using FastAllocated<ListTenantsActor<DB>>::operator new;
	using FastAllocated<ListTenantsActor<DB>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<std::map<TenantName, TenantMapEntry>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ListTenantsActor<DB>, 0, std::map<TenantName, TenantMapEntry> >;
friend struct ActorCallback< ListTenantsActor<DB>, 1, Void >;
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	ListTenantsActor(Reference<DB> const& db,TenantName const& begin,TenantName const& end,int const& limit) 
															#line 7808 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
		 : Actor<std::map<TenantName, TenantMapEntry>>(),
		   ListTenantsActorState<DB, ListTenantsActor<DB>>(db, begin, end, limit)
	{
		fdb_probe_actor_enter("listTenants", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("listTenants");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("listTenants", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ListTenantsActor<DB>, 0, std::map<TenantName, TenantMapEntry> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ListTenantsActor<DB>, 1, Void >*)0, actor_cancelled()); break;
		}

	}
};
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
template <class DB>
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
[[nodiscard]] Future<std::map<TenantName, TenantMapEntry>> listTenants( Reference<DB> const& db, TenantName const& begin, TenantName const& end, int const& limit ) {
															#line 891 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
	return Future<std::map<TenantName, TenantMapEntry>>(new ListTenantsActor<DB>(db, begin, end, limit));
															#line 7838 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.g.h"
}

#line 908 "/usr/src/libfdb_c/fdbclient/GenericManagementAPI.actor.h"
} // namespace ManagementAPI

#include "flow/unactorcompiler.h"
#endif
