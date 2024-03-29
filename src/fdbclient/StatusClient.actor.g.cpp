#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
/*
 * StatusClient.actor.cpp
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

#include "flow/flow.h"
#include "fdbclient/CoordinationInterface.h"
#include "fdbclient/MonitorLeader.h"
#include "fdbclient/ClusterInterface.h"
#include "fdbclient/StatusClient.h"
#include "fdbclient/Status.h"
#include "fdbclient/json_spirit/json_spirit_writer_template.h"
#include "fdbclient/json_spirit/json_spirit_reader_template.h"
#include "fdbrpc/genericactors.actor.h"
#include <cstdint>

#include "flow/actorcompiler.h" // has to be last include

json_spirit::mValue readJSONStrictly(const std::string& s) {
	json_spirit::mValue val;
	std::string::const_iterator i = s.begin();
	if (!json_spirit::read_range(i, s.end(), val)) {
		if (g_network->isSimulated()) {
			printf("MALFORMED: %s\n", s.c_str());
		}
		throw json_malformed();
	}

	// Allow trailing whitespace
	while (i != s.end()) {
		if (!isspace(*i)) {
			if (g_network->isSimulated()) {
				printf(
				    "EXPECTED EOF: %s\n^^^\n%s\n", std::string(s.begin(), i).c_str(), std::string(i, s.end()).c_str());
			}
			throw json_eof_expected();
		}
		++i;
	}

	return val;
}

uint64_t JSONDoc::expires_reference_version = std::numeric_limits<uint64_t>::max();

// Template specializations for mergeOperator
template <>
json_spirit::mObject JSONDoc::mergeOperator<bool>(const std::string& op,
                                                  const json_spirit::mObject& op_a,
                                                  const json_spirit::mObject& op_b,
                                                  bool const& a,
                                                  bool const& b) {
	if (op == "$and")
		return { { op, a && b } };
	if (op == "$or")
		return { { op, a || b } };
	throw std::exception();
}

template <>
json_spirit::mObject JSONDoc::mergeOperator<json_spirit::mArray>(const std::string& op,
                                                                 const json_spirit::mObject& op_a,
                                                                 const json_spirit::mObject& op_b,
                                                                 json_spirit::mArray const& a,
                                                                 json_spirit::mArray const& b) {
	throw std::exception();
}

template <>
json_spirit::mObject JSONDoc::mergeOperator<json_spirit::mObject>(const std::string& op,
                                                                  const json_spirit::mObject& op_a,
                                                                  const json_spirit::mObject& op_b,
                                                                  json_spirit::mObject const& a,
                                                                  json_spirit::mObject const& b) {
	if (op == "$count_keys") {
		json_spirit::mObject combined;
		for (auto& e : a)
			combined[e.first] = json_spirit::mValue();
		for (auto& e : b)
			combined[e.first] = json_spirit::mValue();
		return { { op, combined } };
	}
	throw std::exception();
}

// If the types for a and B differ then pass them as mValues to this specialization.
template <>
json_spirit::mObject JSONDoc::mergeOperator<json_spirit::mValue>(const std::string& op,
                                                                 const json_spirit::mObject& op_a,
                                                                 const json_spirit::mObject& op_b,
                                                                 json_spirit::mValue const& a,
                                                                 json_spirit::mValue const& b) {
	// Returns { $latest : <a or b>, timestamp: <a or b timestamp> }
	// where the thing (a or b) with the highest timestamp operator arg will be chosen
	if (op == "$latest") {
		double ts_a = 0, ts_b = 0;
		JSONDoc(op_a).tryGet("timestamp", ts_a);
		JSONDoc(op_b).tryGet("timestamp", ts_b);
		if (ts_a > ts_b)
			return { { op, a }, { "timestamp", ts_a } };
		return { { op, b }, { "timestamp", ts_b } };
	}

	// Simply selects the last thing to be merged.
	// Returns { $last : b }
	if (op == "$last")
		return { { op, b } };

	// $expires will reduce its value to null if the "version" operator argument is present, nonzero, and has a value
	// that is less than JSONDoc::expires_reference_version.  This DOES mean that if the "version" argument
	// is not present or has a value of 0 then the operator's value will be considered NOT expired.
	// When two $expires operations are merged, the result is
	// { $expires : <value> } where value is the result of a merger between null and any unexpired
	// values for a or b.
	if (op == "$expires") {
		uint64_t ver_a = 0, ver_b = 0;
		// Whichever has the most recent "timestamp" in its operator object will be used
		JSONDoc(op_a).tryGet("version", ver_a);
		JSONDoc(op_b).tryGet("version", ver_b);

		json_spirit::mValue r;
		// If version is 0 or greater than the current reference version then use the value
		if (ver_a == 0 || ver_a > JSONDoc::expires_reference_version)
			r = a;
		if (ver_b == 0 || ver_b > JSONDoc::expires_reference_version)
			mergeValueInto(r, b);

		return { { op, r } };
	}

	throw std::exception();
}

void JSONDoc::cleanOps(json_spirit::mObject& obj) {
	auto kv = obj.begin();
	while (kv != obj.end()) {
		if (kv->second.type() == json_spirit::obj_type) {
			json_spirit::mObject& o = kv->second.get_obj();
			std::string op = getOperator(o);
			// If an operator was found, replace object with its value.
			if (!op.empty()) {
				// The "count_keys" operator needs special handling
				if (op == "$count_keys") {
					int count = 1;
					if (o.at(op).type() == json_spirit::obj_type)
						count = o.at(op).get_obj().size();
					kv->second = count;
				} else if (op == "$expires") {
					uint64_t version = 0;
					JSONDoc(o).tryGet("version", version);
					if (version == 0 || version > JSONDoc::expires_reference_version)
						kv->second = o.at(op);
					else {
						// Thing is expired so competely remove its key from the enclosing Object
						auto tmp = kv;
						++kv;
						obj.erase(tmp);
					}
				} else // For others just move the value to replace the operator object
					kv->second = o.at(op);
				// Don't advance kv because the new value could also be an operator
				continue;
			} else {
				// It's not an operator, just a regular object so clean it too.
				cleanOps(o);
			}
		}
		++kv;
	}
}

void JSONDoc::mergeInto(json_spirit::mObject& dst, const json_spirit::mObject& src) {
	for (auto& i : src) {
		// printf("Merging key: %s\n", i.first.c_str());
		mergeValueInto(dst[i.first], i.second);
	}
}

void JSONDoc::mergeValueInto(json_spirit::mValue& dst, const json_spirit::mValue& src) {
	if (src.is_null())
		return;

	if (dst.is_null()) {
		dst = src;
		return;
	}

	// Do nothing if d is already an error
	if (dst.type() == json_spirit::obj_type && dst.get_obj().count("ERROR"))
		return;

	if (dst.type() != src.type()) {
		dst = json_spirit::mObject({ { "ERROR", "Incompatible types." }, { "a", dst }, { "b", src } });
		return;
	}

	switch (dst.type()) {
	case json_spirit::obj_type: {
		// Refs to the objects, for convenience.
		json_spirit::mObject& aObj = dst.get_obj();
		const json_spirit::mObject& bObj = src.get_obj();

		const std::string& op = getOperator(aObj);
		const std::string& opB = getOperator(bObj);

		// Operators must be the same, which could mean both are empty (if these objects are not operators)
		if (op != opB) {
			dst = json_spirit::mObject({ { "ERROR", "Operators do not match" }, { "a", dst }, { "b", src } });
			break;
		}

		// If objects are not operators then defer to mergeInto
		if (op.empty()) {
			mergeInto(dst.get_obj(), src.get_obj());
			break;
		}

		// Get the operator values
		json_spirit::mValue& a = aObj.at(op);
		const json_spirit::mValue& b = bObj.at(op);

		// First try the operators that are type-agnostic
		try {
			dst = mergeOperator<json_spirit::mValue>(op, aObj, bObj, a, b);
			return;
		} catch (std::exception&) {
		}

		// Now try type and type pair specific operators
		// First, if types are incompatible try to make them compatible or return an error
		if (a.type() != b.type()) {
			// It's actually okay if the type mismatch is double vs int since once can be converted to the other.
			if ((a.type() == json_spirit::int_type && b.type() == json_spirit::real_type) ||
			    (b.type() == json_spirit::int_type && a.type() == json_spirit::real_type)) {
				// Convert d's op value (which a is a reference to) to a double so that the
				// switch block below will do the operation with doubles.
				a = a.get_real();
			} else {
				// Otherwise, output an error as the types do not match
				dst = json_spirit::mObject(
				    { { "ERROR", "Incompatible operator value types" }, { "a", dst }, { "b", src } });
				return;
			}
		}

		// Now try the type-specific operators.
		try {
			switch (a.type()) {
			case json_spirit::bool_type:
				dst = mergeOperatorWrapper<bool>(op, aObj, bObj, a, b);
				break;
			case json_spirit::int_type:
				dst = mergeOperatorWrapper<int64_t>(op, aObj, bObj, a, b);
				break;
			case json_spirit::real_type:
				dst = mergeOperatorWrapper<double>(op, aObj, bObj, a, b);
				break;
			case json_spirit::str_type:
				dst = mergeOperatorWrapper<std::string>(op, aObj, bObj, a, b);
				break;
			case json_spirit::array_type:
				dst = mergeOperatorWrapper<json_spirit::mArray>(op, aObj, bObj, a, b);
				break;
			case json_spirit::obj_type:
				dst = mergeOperatorWrapper<json_spirit::mObject>(op, aObj, bObj, a, b);
				break;
			case json_spirit::null_type:
				break;
			}
		} catch (...) {
			dst = json_spirit::mObject({ { "ERROR", "Unsupported operator / value type combination." },
			                             { "operator", op },
			                             { "type", a.type() } });
		}
		break;
	}

	case json_spirit::array_type:
		for (auto& ai : src.get_array())
			dst.get_array().push_back(ai);
		break;

	default:
		if (!(dst == src))
			dst = json_spirit::mObject({ { "ERROR", "Values do not match." }, { "a", dst }, { "b", src } });
	}
}

// Check if a quorum of coordination servers is reachable
// Will not throw, will just return non-present Optional if error
															#line 308 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
namespace {
// This generated class is to be used only via clientCoordinatorsStatusFetcher()
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
template <class ClientCoordinatorsStatusFetcherActor>
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClientCoordinatorsStatusFetcherActorState {
															#line 315 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClientCoordinatorsStatusFetcherActorState(Reference<IClusterConnectionRecord> const& connRecord,bool* const& quorum_reachable,int* const& coordinatorsFaultTolerance) 
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		 : connRecord(connRecord),
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   quorum_reachable(quorum_reachable),
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   coordinatorsFaultTolerance(coordinatorsFaultTolerance)
															#line 326 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
	{
		fdb_probe_actor_create("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	~ClientCoordinatorsStatusFetcherActorState() 
	{
		fdb_probe_actor_destroy("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			try {
															#line 310 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coord = ClientCoordinators(connRecord);
															#line 311 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				statusObj = StatusObject();
															#line 313 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				leaderServers = std::vector<Future<Optional<LeaderInfo>>>();
															#line 314 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				leaderServers.reserve(coord.clientLeaderServers.size());
															#line 315 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 316 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					if (coord.clientLeaderServers[i].hostname.present())
															#line 352 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					{
															#line 317 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
						leaderServers.push_back(retryGetReplyFromHostname(GetLeaderRequest(coord.clusterKey, UID()), coord.clientLeaderServers[i].hostname.get(), WLTOKEN_CLIENTLEADERREG_GETLEADER, TaskPriority::CoordinationReply));
															#line 356 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					}
					else
					{
															#line 322 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
						leaderServers.push_back(retryBrokenPromise(coord.clientLeaderServers[i].getLeader, GetLeaderRequest(coord.clusterKey, UID()), TaskPriority::CoordinationReply));
															#line 362 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					}
				}
															#line 328 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordProtocols = std::vector<Future<ProtocolInfoReply>>();
															#line 329 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordProtocols.reserve(coord.clientLeaderServers.size());
															#line 330 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				for(int i = 0;i < coord.clientLeaderServers.size();i++) {
															#line 331 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					if (coord.clientLeaderServers[i].hostname.present())
															#line 373 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					{
															#line 332 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
						coordProtocols.push_back(retryGetReplyFromHostname( ProtocolInfoRequest{}, coord.clientLeaderServers[i].hostname.get(), WLTOKEN_PROTOCOL_INFO));
															#line 377 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					}
					else
					{
															#line 335 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
						RequestStream<ProtocolInfoRequest> requestStream{ Endpoint::wellKnown( { coord.clientLeaderServers[i].getLeader.getEndpoint().addresses }, WLTOKEN_PROTOCOL_INFO) };
															#line 337 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
						coordProtocols.push_back(retryBrokenPromise(requestStream, ProtocolInfoRequest{}));
															#line 385 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
					}
				}
															#line 341 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StrictFuture<Void> __when_expr_0 = smartQuorum(leaderServers, leaderServers.size() / 2 + 1, 1.5) && smartQuorum(coordProtocols, coordProtocols.size() / 2 + 1, 1.5) || delay(2.0);
															#line 341 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->actor_wait_state = 1;
															#line 341 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >*>(static_cast<ClientCoordinatorsStatusFetcherActor*>(this)));
															#line 397 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
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
		this->~ClientCoordinatorsStatusFetcherActorState();
		static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 379 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			*quorum_reachable = false;
															#line 380 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (!static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV<Optional<StatusObject>>::futures) { (void)(Optional<StatusObject>()); this->~ClientCoordinatorsStatusFetcherActorState(); static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->destroy(); return 0; }
															#line 429 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			new (&static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV< Optional<StatusObject> >::value()) Optional<StatusObject>(Optional<StatusObject>());
			this->~ClientCoordinatorsStatusFetcherActorState();
			static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont2(Void const& _,int loopDepth) 
	{
															#line 345 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["quorum_reachable"] = *quorum_reachable = quorum(leaderServers, leaderServers.size() / 2 + 1).isReady();
															#line 348 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StatusArray coordsStatus;
															#line 349 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		int coordinatorsUnavailable = 0;
															#line 350 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		for(int i = 0;i < leaderServers.size();i++) {
															#line 351 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StatusObject coordStatus;
															#line 352 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (coord.clientLeaderServers[i].hostname.present())
															#line 457 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 353 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["address"] = coord.clientLeaderServers[i].hostname.get().toString();
															#line 461 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 355 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["address"] = coord.clientLeaderServers[i].getLeader.getEndpoint().getPrimaryAddress().toString();
															#line 467 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 359 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (leaderServers[i].isReady())
															#line 471 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 360 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["reachable"] = true;
															#line 475 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 362 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordinatorsUnavailable++;
															#line 363 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["reachable"] = false;
															#line 483 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 365 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (coordProtocols[i].isReady())
															#line 487 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 366 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				uint64_t protocolVersionInt = coordProtocols[i].get().version.version();
															#line 367 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				std::stringstream hexSs;
															#line 368 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				hexSs << std::hex << std::setw(2 * sizeof(protocolVersionInt)) << std::setfill('0') << protocolVersionInt;
															#line 370 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["protocol"] = hexSs.str();
															#line 497 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 372 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			coordsStatus.push_back(coordStatus);
															#line 501 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
															#line 374 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["coordinators"] = coordsStatus;
															#line 376 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		*coordinatorsFaultTolerance = (leaderServers.size() - 1) / 2 - coordinatorsUnavailable;
															#line 377 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV<Optional<StatusObject>>::futures) { (void)(statusObj); this->~ClientCoordinatorsStatusFetcherActorState(); static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->destroy(); return 0; }
															#line 509 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		new (&static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV< Optional<StatusObject> >::value()) Optional<StatusObject>(std::move(statusObj)); // state_var_RVO
		this->~ClientCoordinatorsStatusFetcherActorState();
		static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont2(Void && _,int loopDepth) 
	{
															#line 345 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["quorum_reachable"] = *quorum_reachable = quorum(leaderServers, leaderServers.size() / 2 + 1).isReady();
															#line 348 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StatusArray coordsStatus;
															#line 349 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		int coordinatorsUnavailable = 0;
															#line 350 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		for(int i = 0;i < leaderServers.size();i++) {
															#line 351 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StatusObject coordStatus;
															#line 352 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (coord.clientLeaderServers[i].hostname.present())
															#line 531 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 353 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["address"] = coord.clientLeaderServers[i].hostname.get().toString();
															#line 535 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 355 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["address"] = coord.clientLeaderServers[i].getLeader.getEndpoint().getPrimaryAddress().toString();
															#line 541 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 359 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (leaderServers[i].isReady())
															#line 545 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 360 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["reachable"] = true;
															#line 549 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 362 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordinatorsUnavailable++;
															#line 363 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["reachable"] = false;
															#line 557 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 365 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (coordProtocols[i].isReady())
															#line 561 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 366 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				uint64_t protocolVersionInt = coordProtocols[i].get().version.version();
															#line 367 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				std::stringstream hexSs;
															#line 368 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				hexSs << std::hex << std::setw(2 * sizeof(protocolVersionInt)) << std::setfill('0') << protocolVersionInt;
															#line 370 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				coordStatus["protocol"] = hexSs.str();
															#line 571 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 372 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			coordsStatus.push_back(coordStatus);
															#line 575 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
															#line 374 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["coordinators"] = coordsStatus;
															#line 376 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		*coordinatorsFaultTolerance = (leaderServers.size() - 1) / 2 - coordinatorsUnavailable;
															#line 377 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV<Optional<StatusObject>>::futures) { (void)(statusObj); this->~ClientCoordinatorsStatusFetcherActorState(); static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->destroy(); return 0; }
															#line 583 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		new (&static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->SAV< Optional<StatusObject> >::value()) Optional<StatusObject>(std::move(statusObj)); // state_var_RVO
		this->~ClientCoordinatorsStatusFetcherActorState();
		static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont2(_, loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont2(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClientCoordinatorsStatusFetcherActor*>(this)->ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	bool* quorum_reachable;
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	int* coordinatorsFaultTolerance;
															#line 310 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClientCoordinators coord;
															#line 311 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusObject statusObj;
															#line 313 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	std::vector<Future<Optional<LeaderInfo>>> leaderServers;
															#line 328 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	std::vector<Future<ProtocolInfoReply>> coordProtocols;
															#line 668 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
};
// This generated class is to be used only via clientCoordinatorsStatusFetcher()
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClientCoordinatorsStatusFetcherActor final : public Actor<Optional<StatusObject>>, public ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >, public FastAllocated<ClientCoordinatorsStatusFetcherActor>, public ClientCoordinatorsStatusFetcherActorState<ClientCoordinatorsStatusFetcherActor> {
															#line 673 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
	using FastAllocated<ClientCoordinatorsStatusFetcherActor>::operator new;
	using FastAllocated<ClientCoordinatorsStatusFetcherActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<StatusObject>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >;
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClientCoordinatorsStatusFetcherActor(Reference<IClusterConnectionRecord> const& connRecord,bool* const& quorum_reachable,int* const& coordinatorsFaultTolerance) 
															#line 684 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		 : Actor<Optional<StatusObject>>(),
		   ClientCoordinatorsStatusFetcherActorState<ClientCoordinatorsStatusFetcherActor>(connRecord, quorum_reachable, coordinatorsFaultTolerance)
	{
		fdb_probe_actor_enter("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("clientCoordinatorsStatusFetcher");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("clientCoordinatorsStatusFetcher", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClientCoordinatorsStatusFetcherActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
[[nodiscard]] Future<Optional<StatusObject>> clientCoordinatorsStatusFetcher( Reference<IClusterConnectionRecord> const& connRecord, bool* const& quorum_reachable, int* const& coordinatorsFaultTolerance ) {
															#line 306 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	return Future<Optional<StatusObject>>(new ClientCoordinatorsStatusFetcherActor(connRecord, quorum_reachable, coordinatorsFaultTolerance));
															#line 712 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
}

#line 383 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"

// Client section of the json output
// Will NOT throw, errors will be put into messages array
															#line 719 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
namespace {
// This generated class is to be used only via clientStatusFetcher()
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
template <class ClientStatusFetcherActor>
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClientStatusFetcherActorState {
															#line 726 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClientStatusFetcherActorState(Reference<IClusterConnectionRecord> const& connRecord,StatusArray* const& messages,bool* const& quorum_reachable,int* const& coordinatorsFaultTolerance) 
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		 : connRecord(connRecord),
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   messages(messages),
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   quorum_reachable(quorum_reachable),
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   coordinatorsFaultTolerance(coordinatorsFaultTolerance),
															#line 390 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   statusObj()
															#line 741 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
	{
		fdb_probe_actor_create("clientStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	~ClientStatusFetcherActorState() 
	{
		fdb_probe_actor_destroy("clientStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StrictFuture<Optional<StatusObject>> __when_expr_0 = clientCoordinatorsStatusFetcher(connRecord, quorum_reachable, coordinatorsFaultTolerance);
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 758 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 1;
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >*>(static_cast<ClientStatusFetcherActor*>(this)));
															#line 763 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
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
		this->~ClientStatusFetcherActorState();
		static_cast<ClientStatusFetcherActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 394 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<bool> __when_expr_1 = connRecord->upToDate();
															#line 394 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 788 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1cont1when1(__when_expr_1.get(), loopDepth); };
		static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 2;
															#line 394 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClientStatusFetcherActor, 1, bool >*>(static_cast<ClientStatusFetcherActor*>(this)));
															#line 793 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1when1(Optional<StatusObject> const& __coordsStatusObj,int loopDepth) 
	{
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		coordsStatusObj = __coordsStatusObj;
															#line 802 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Optional<StatusObject> && __coordsStatusObj,int loopDepth) 
	{
		coordsStatusObj = std::move(__coordsStatusObj);
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClientStatusFetcherActor*>(this)->ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >::remove();

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >*,Optional<StatusObject> const& value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >*,Optional<StatusObject> && value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >*,Error err) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 396 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (coordsStatusObj.present())
															#line 869 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 397 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObj["coordinators"] = coordsStatusObj.get();
															#line 398 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (!*quorum_reachable)
															#line 875 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 399 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				messages->push_back(makeMessage("quorum_not_reachable", "Unable to reach a quorum of coordinators."));
															#line 879 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
		}
		else
		{
															#line 401 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			messages->push_back(makeMessage("status_incomplete_coordinators", "Could not fetch coordinator info."));
															#line 886 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
															#line 403 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StatusObject statusObjClusterFile;
															#line 404 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClusterFile["path"] = connRecord->getLocation();
															#line 405 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClusterFile["up_to_date"] = contentsUpToDate;
															#line 406 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["cluster_file"] = statusObjClusterFile;
															#line 408 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!contentsUpToDate)
															#line 898 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 409 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StrictFuture<ClusterConnectionString> __when_expr_2 = connRecord->getStoredConnectionString();
															#line 409 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 904 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont2when1(__when_expr_2.get(), loopDepth); };
			static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 3;
															#line 409 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >*>(static_cast<ClientStatusFetcherActor*>(this)));
															#line 909 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont3(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1when1(bool const& __contentsUpToDate,int loopDepth) 
	{
															#line 394 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		contentsUpToDate = __contentsUpToDate;
															#line 923 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(bool && __contentsUpToDate,int loopDepth) 
	{
		contentsUpToDate = std::move(__contentsUpToDate);
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClientStatusFetcherActor*>(this)->ActorCallback< ClientStatusFetcherActor, 1, bool >::remove();

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 1, bool >*,bool const& value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 1, bool >*,bool && value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClientStatusFetcherActor, 1, bool >*,Error err) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont3(int loopDepth) 
	{
															#line 423 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!static_cast<ClientStatusFetcherActor*>(this)->SAV<StatusObject>::futures) { (void)(statusObj); this->~ClientStatusFetcherActorState(); static_cast<ClientStatusFetcherActor*>(this)->destroy(); return 0; }
															#line 990 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		new (&static_cast<ClientStatusFetcherActor*>(this)->SAV< StatusObject >::value()) StatusObject(std::move(statusObj)); // state_var_RVO
		this->~ClientStatusFetcherActorState();
		static_cast<ClientStatusFetcherActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(ClusterConnectionString const& storedConnectionString,int loopDepth) 
	{
															#line 410 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		std::string description = "Cluster file contents do not match current cluster connection string.";
															#line 411 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nThe file contains the connection string: ";
															#line 412 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += storedConnectionString.toString().c_str();
															#line 413 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nThe current connection string is: ";
															#line 414 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += connRecord->getConnectionString().toString().c_str();
															#line 415 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nVerify the cluster file and its parent directory are writable and that the cluster file has " "not been overwritten externally. To change coordinators without manual intervention, the " "cluster file and its containing folder must be writable by all servers and clients. If a " "majority of the coordinators referenced by the old connection string are lost, the database " "will stop working until the correct cluster file is distributed to all processes.";
															#line 420 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		messages->push_back(makeMessage("incorrect_cluster_file_contents", description.c_str()));
															#line 1014 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont7(ClusterConnectionString && storedConnectionString,int loopDepth) 
	{
															#line 410 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		std::string description = "Cluster file contents do not match current cluster connection string.";
															#line 411 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nThe file contains the connection string: ";
															#line 412 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += storedConnectionString.toString().c_str();
															#line 413 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nThe current connection string is: ";
															#line 414 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += connRecord->getConnectionString().toString().c_str();
															#line 415 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		description += "\nVerify the cluster file and its parent directory are writable and that the cluster file has " "not been overwritten externally. To change coordinators without manual intervention, the " "cluster file and its containing folder must be writable by all servers and clients. If a " "majority of the coordinators referenced by the old connection string are lost, the database " "will stop working until the correct cluster file is distributed to all processes.";
															#line 420 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		messages->push_back(makeMessage("incorrect_cluster_file_contents", description.c_str()));
															#line 1035 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont3(loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(ClusterConnectionString const& storedConnectionString,int loopDepth) 
	{
		loopDepth = a_body1cont7(storedConnectionString, loopDepth);

		return loopDepth;
	}
	int a_body1cont2when1(ClusterConnectionString && storedConnectionString,int loopDepth) 
	{
		loopDepth = a_body1cont7(std::move(storedConnectionString), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClientStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClientStatusFetcherActor*>(this)->ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >::remove();

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >*,ClusterConnectionString const& value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >*,ClusterConnectionString && value) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >*,Error err) 
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusArray* messages;
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	bool* quorum_reachable;
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	int* coordinatorsFaultTolerance;
															#line 390 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusObject statusObj;
															#line 392 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Optional<StatusObject> coordsStatusObj;
															#line 394 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	bool contentsUpToDate;
															#line 1117 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
};
// This generated class is to be used only via clientStatusFetcher()
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClientStatusFetcherActor final : public Actor<StatusObject>, public ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >, public ActorCallback< ClientStatusFetcherActor, 1, bool >, public ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >, public FastAllocated<ClientStatusFetcherActor>, public ClientStatusFetcherActorState<ClientStatusFetcherActor> {
															#line 1122 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
	using FastAllocated<ClientStatusFetcherActor>::operator new;
	using FastAllocated<ClientStatusFetcherActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<StatusObject>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >;
friend struct ActorCallback< ClientStatusFetcherActor, 1, bool >;
friend struct ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >;
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClientStatusFetcherActor(Reference<IClusterConnectionRecord> const& connRecord,StatusArray* const& messages,bool* const& quorum_reachable,int* const& coordinatorsFaultTolerance) 
															#line 1135 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		 : Actor<StatusObject>(),
		   ClientStatusFetcherActorState<ClientStatusFetcherActor>(connRecord, messages, quorum_reachable, coordinatorsFaultTolerance)
	{
		fdb_probe_actor_enter("clientStatusFetcher", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("clientStatusFetcher");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("clientStatusFetcher", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClientStatusFetcherActor, 0, Optional<StatusObject> >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClientStatusFetcherActor, 1, bool >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< ClientStatusFetcherActor, 2, ClusterConnectionString >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
[[nodiscard]] Future<StatusObject> clientStatusFetcher( Reference<IClusterConnectionRecord> const& connRecord, StatusArray* const& messages, bool* const& quorum_reachable, int* const& coordinatorsFaultTolerance ) {
															#line 386 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	return Future<StatusObject>(new ClientStatusFetcherActor(connRecord, messages, quorum_reachable, coordinatorsFaultTolerance));
															#line 1165 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
}

#line 425 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"

// Cluster section of json output
															#line 1171 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
namespace {
// This generated class is to be used only via clusterStatusFetcher()
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
template <class ClusterStatusFetcherActor>
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClusterStatusFetcherActorState {
															#line 1178 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClusterStatusFetcherActorState(ClusterInterface const& cI,StatusArray* const& messages) 
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		 : cI(cI),
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   messages(messages),
															#line 428 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   req(),
															#line 429 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   clusterTimeout(delay(30.0)),
															#line 430 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   oStatusObj()
															#line 1193 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
	{
		fdb_probe_actor_create("clusterStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	~ClusterStatusFetcherActorState() 
	{
		fdb_probe_actor_destroy("clusterStatusFetcher", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 432 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StrictFuture<Void> __when_expr_0 = delay(0.0);
															#line 432 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 1210 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
			static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state = 1;
															#line 432 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< ClusterStatusFetcherActor, 0, Void >*>(static_cast<ClusterStatusFetcherActor*>(this)));
															#line 1215 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
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
		this->~ClusterStatusFetcherActorState();
		static_cast<ClusterStatusFetcherActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(Void const& _,int loopDepth) 
	{
															#line 434 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusReply = cI.databaseStatus.tryGetReply(req);
															#line 435 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		;
															#line 1240 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1(Void && _,int loopDepth) 
	{
															#line 434 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusReply = cI.databaseStatus.tryGetReply(req);
															#line 435 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		;
															#line 1251 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont1loopHead1(loopDepth);

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
		if (static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterStatusFetcherActor*>(this)->ActorCallback< ClusterStatusFetcherActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< ClusterStatusFetcherActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 463 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!static_cast<ClusterStatusFetcherActor*>(this)->SAV<Optional<StatusObject>>::futures) { (void)(oStatusObj); this->~ClusterStatusFetcherActorState(); static_cast<ClusterStatusFetcherActor*>(this)->destroy(); return 0; }
															#line 1323 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		new (&static_cast<ClusterStatusFetcherActor*>(this)->SAV< Optional<StatusObject> >::value()) Optional<StatusObject>(std::move(oStatusObj)); // state_var_RVO
		this->~ClusterStatusFetcherActorState();
		static_cast<ClusterStatusFetcherActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 437 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<ErrorOr<StatusReply>> __when_expr_1 = statusReply;
															#line 436 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1344 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
															#line 456 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<Void> __when_expr_2 = clusterTimeout;
															#line 1348 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when2(__when_expr_2.get(), loopDepth); };
		static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state = 2;
															#line 437 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >*>(static_cast<ClusterStatusFetcherActor*>(this)));
															#line 456 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< ClusterStatusFetcherActor, 2, Void >*>(static_cast<ClusterStatusFetcherActor*>(this)));
															#line 1355 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont2(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(ErrorOr<StatusReply> const& result,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (result.isError())
															#line 1377 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 439 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (result.getError().code() == error_code_request_maybe_delivered)
															#line 1381 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 440 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				messages->push_back(makeMessage("unreachable_cluster_controller", ("Unable to communicate with the cluster controller at " + cI.address().toString() + " to get status.") .c_str()));
															#line 1385 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 444 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (result.getError().code() == error_code_server_overloaded)
															#line 1391 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				{
															#line 445 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					messages->push_back(makeMessage("server_overloaded", "The cluster controller is currently processing too many " "status requests and is unable to respond"));
															#line 1395 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
				else
				{
															#line 449 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					messages->push_back( makeMessage("status_incomplete_error", "Cluster encountered an error fetching status."));
															#line 1401 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 452 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			oStatusObj = result.get().statusObj;
															#line 1409 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(ErrorOr<StatusReply> && result,int loopDepth) 
	{
															#line 438 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (result.isError())
															#line 1419 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 439 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (result.getError().code() == error_code_request_maybe_delivered)
															#line 1423 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 440 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				messages->push_back(makeMessage("unreachable_cluster_controller", ("Unable to communicate with the cluster controller at " + cI.address().toString() + " to get status.") .c_str()));
															#line 1427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
			else
			{
															#line 444 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (result.getError().code() == error_code_server_overloaded)
															#line 1433 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				{
															#line 445 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					messages->push_back(makeMessage("server_overloaded", "The cluster controller is currently processing too many " "status requests and is unable to respond"));
															#line 1437 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
				else
				{
															#line 449 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					messages->push_back( makeMessage("status_incomplete_error", "Cluster encountered an error fetching status."));
															#line 1443 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
			}
		}
		else
		{
															#line 452 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			oStatusObj = result.get().statusObj;
															#line 1451 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 457 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		messages->push_back(makeMessage("status_incomplete_timeout", "Timed out fetching cluster status."));
															#line 1461 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 457 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		messages->push_back(makeMessage("status_incomplete_timeout", "Timed out fetching cluster status."));
															#line 1470 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state > 0) static_cast<ClusterStatusFetcherActor*>(this)->actor_wait_state = 0;
		static_cast<ClusterStatusFetcherActor*>(this)->ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >::remove();
		static_cast<ClusterStatusFetcherActor*>(this)->ActorCallback< ClusterStatusFetcherActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >*,ErrorOr<StatusReply> const& value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >*,ErrorOr<StatusReply> && value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >*,Error err) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< ClusterStatusFetcherActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< ClusterStatusFetcherActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClusterInterface cI;
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusArray* messages;
															#line 428 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusRequest req;
															#line 429 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Future<Void> clusterTimeout;
															#line 430 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Optional<StatusObject> oStatusObj;
															#line 434 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Future<ErrorOr<StatusReply>> statusReply;
															#line 1584 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
};
// This generated class is to be used only via clusterStatusFetcher()
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class ClusterStatusFetcherActor final : public Actor<Optional<StatusObject>>, public ActorCallback< ClusterStatusFetcherActor, 0, Void >, public ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >, public ActorCallback< ClusterStatusFetcherActor, 2, Void >, public FastAllocated<ClusterStatusFetcherActor>, public ClusterStatusFetcherActorState<ClusterStatusFetcherActor> {
															#line 1589 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
	using FastAllocated<ClusterStatusFetcherActor>::operator new;
	using FastAllocated<ClusterStatusFetcherActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Optional<StatusObject>>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< ClusterStatusFetcherActor, 0, Void >;
friend struct ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >;
friend struct ActorCallback< ClusterStatusFetcherActor, 2, Void >;
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	ClusterStatusFetcherActor(ClusterInterface const& cI,StatusArray* const& messages) 
															#line 1602 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		 : Actor<Optional<StatusObject>>(),
		   ClusterStatusFetcherActorState<ClusterStatusFetcherActor>(cI, messages)
	{
		fdb_probe_actor_enter("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("clusterStatusFetcher");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("clusterStatusFetcher", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< ClusterStatusFetcherActor, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< ClusterStatusFetcherActor, 1, ErrorOr<StatusReply> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
[[nodiscard]] Future<Optional<StatusObject>> clusterStatusFetcher( ClusterInterface const& cI, StatusArray* const& messages ) {
															#line 427 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	return Future<Optional<StatusObject>>(new ClusterStatusFetcherActor(cI, messages));
															#line 1631 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
}

#line 465 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"

// Create and return a database_status section.
// Will not throw, will not return an empty section.
StatusObject getClientDatabaseStatus(StatusObjectReader client, StatusObjectReader cluster) {
	bool isAvailable = false;
	bool isHealthy = false;

	try {
		// Lots of the JSON reads in this code could throw, and that's OK, isAvailable and isHealthy will be
		// at the states we want them to be in (currently)
		std::string recoveryStateName = cluster.at("recovery_state.name").get_str();
		isAvailable = client.at("coordinators.quorum_reachable").get_bool() &&
		              (recoveryStateName == "accepting_commits" || recoveryStateName == "all_logs_recruited" ||
		               recoveryStateName == "storage_recovered" || recoveryStateName == "fully_recovered") &&
		              cluster.at("database_available").get_bool();

		if (isAvailable) {
			bool procMessagesPresent = false;
			// OK to throw if processes doesn't exist, can't have an available database without processes
			for (auto p : cluster.at("processes").get_obj()) {
				StatusObjectReader proc(p.second);
				if (proc.has("messages") && proc.last().get_array().size()) {
					procMessagesPresent = true;
					break;
				}
			}

			bool data_state_present = cluster.has("data.state");

			bool data_state_unhealthy =
			    data_state_present && cluster.has("data.state.healthy") && !cluster.last().get_bool();

			int cluster_messages = cluster.has("messages") ? cluster.last().get_array().size() : 0;
			int configuration_messages = client.has("configuration.messages") ? client.last().get_array().size() : 0;

			isHealthy =
			    !(cluster_messages > 0 || configuration_messages > 0 || procMessagesPresent || data_state_unhealthy ||
			      !data_state_present || !client.at("cluster_file.up_to_date").get_bool());
		}
	} catch (std::exception&) {
		// As documented above, exceptions leave isAvailable and isHealthy in the right state
	}

	StatusObject databaseStatus;
	databaseStatus["healthy"] = isHealthy;
	databaseStatus["available"] = isAvailable;
	return databaseStatus;
}

															#line 1684 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
namespace {
// This generated class is to be used only via statusFetcherImpl()
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
template <class StatusFetcherImplActor>
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class StatusFetcherImplActorState {
															#line 1691 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusFetcherImplActorState(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Optional<ClusterInterface>>> const& clusterInterface) 
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		 : connRecord(connRecord),
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   clusterInterface(clusterInterface)
															#line 1700 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
	{
		fdb_probe_actor_create("statusFetcherImpl", reinterpret_cast<unsigned long>(this));

	}
	~StatusFetcherImplActorState() 
	{
		fdb_probe_actor_destroy("statusFetcherImpl", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 516 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (!g_network)
															#line 1715 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 517 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				return a_body1Catch1(network_not_setup(), loopDepth);
															#line 1719 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 519 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObj = StatusObject();
															#line 520 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObjClient = StatusObject();
															#line 521 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			clientMessages = StatusArray();
															#line 524 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			quorum_reachable = false;
															#line 525 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			coordinatorsFaultTolerance = 0;
															#line 1731 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			try {
															#line 528 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				clientTime = g_network->timer();
															#line 530 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StrictFuture<StatusObject> __when_expr_0 = clientStatusFetcher(connRecord, &clientMessages, &quorum_reachable, &coordinatorsFaultTolerance);
															#line 530 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state < 0) return a_body1Catch2(actor_cancelled(), loopDepth);
															#line 1739 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch2(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
				static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 1;
															#line 530 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< StatusFetcherImplActor, 0, StatusObject >*>(static_cast<StatusFetcherImplActor*>(this)));
															#line 1744 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
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
		this->~StatusFetcherImplActorState();
		static_cast<StatusFetcherImplActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 547 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjCluster = StatusObject();
															#line 549 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (quorum_reachable)
															#line 1775 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
			try {
															#line 552 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				interfaceTimeout = delay(2.0);
															#line 554 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				;
															#line 1782 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				loopDepth = a_body1cont1loopHead1(loopDepth);
			}
			catch (Error& error) {
				loopDepth = a_body1cont1Catch1(error, loopDepth);
			} catch (...) {
				loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont7(loopDepth);
		}

		return loopDepth;
	}
	int a_body1Catch2(const Error& e,int loopDepth=0) 
	{
		try {
															#line 537 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (e.code() == error_code_actor_cancelled)
															#line 1803 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 538 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				return a_body1Catch1(e, loopDepth);
															#line 1807 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			}
															#line 539 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			TraceEvent(SevError, "ClientStatusFetchError").error(e);
															#line 540 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			clientMessages.push_back( makeMessage("status_incomplete_client", "Could not retrieve client status information."));
															#line 1813 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			loopDepth = a_body1cont1(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont3(StatusObject const& _statusObjClient,int loopDepth) 
	{
															#line 532 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClient = _statusObjClient;
															#line 534 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (clientTime != -1)
															#line 1830 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 535 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObjClient["timestamp"] = clientTime;
															#line 1834 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1cont3(StatusObject && _statusObjClient,int loopDepth) 
	{
															#line 532 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClient = _statusObjClient;
															#line 534 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (clientTime != -1)
															#line 1846 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 535 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObjClient["timestamp"] = clientTime;
															#line 1850 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
		loopDepth = a_body1cont6(loopDepth);

		return loopDepth;
	}
	int a_body1when1(StatusObject const& _statusObjClient,int loopDepth) 
	{
		loopDepth = a_body1cont3(_statusObjClient, loopDepth);

		return loopDepth;
	}
	int a_body1when1(StatusObject && _statusObjClient,int loopDepth) 
	{
		loopDepth = a_body1cont3(std::move(_statusObjClient), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state > 0) static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 0;
		static_cast<StatusFetcherImplActor*>(this)->ActorCallback< StatusFetcherImplActor, 0, StatusObject >::remove();

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 0, StatusObject >*,StatusObject const& value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 0, StatusObject >*,StatusObject && value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< StatusFetcherImplActor, 0, StatusObject >*,Error err) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch2(err, 0);
		}
		catch (Error& error) {
			a_body1Catch2(error, 0);
		} catch (...) {
			a_body1Catch2(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1cont6(int loopDepth) 
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
	int a_body1cont7(int loopDepth) 
	{
															#line 603 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClient["messages"] = clientMessages;
															#line 606 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObjClient["database_status"] = getClientDatabaseStatus(statusObjClient, statusObjCluster);
															#line 609 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["client"] = statusObjClient;
															#line 612 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		JSONDoc doc(statusObj);
															#line 613 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		auto& layers_valid = doc.create("cluster.layers._valid");
															#line 614 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (layers_valid.is_null())
															#line 1946 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 615 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			layers_valid = false;
															#line 1950 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		}
															#line 617 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (!static_cast<StatusFetcherImplActor*>(this)->SAV<StatusObject>::futures) { (void)(statusObj); this->~StatusFetcherImplActorState(); static_cast<StatusFetcherImplActor*>(this)->destroy(); return 0; }
															#line 1954 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		new (&static_cast<StatusFetcherImplActor*>(this)->SAV< StatusObject >::value()) StatusObject(std::move(statusObj)); // state_var_RVO
		this->~StatusFetcherImplActorState();
		static_cast<StatusFetcherImplActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont1Catch1(const Error& e,int loopDepth=0) 
	{
		try {
															#line 594 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			TraceEvent(e.code() == error_code_all_alternatives_failed ? SevInfo : SevError, "ClusterStatusFetchError") .error(e);
															#line 597 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			clientMessages.push_back( makeMessage("status_incomplete_cluster", "Could not retrieve cluster status information."));
															#line 1975 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			loopDepth = a_body1cont8(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont9(int loopDepth) 
	{
															#line 592 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		statusObj["cluster"] = statusObjCluster;
															#line 1990 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = a_body1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1cont1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1(int loopDepth) 
	{
															#line 555 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (clusterInterface->get().present())
															#line 2006 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 556 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			StrictFuture<Optional<StatusObject>> __when_expr_1 = clusterStatusFetcher(clusterInterface->get().get(), &clientMessages);
															#line 556 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2012 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1cont1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1when1(__when_expr_1.get(), loopDepth); };
			static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 2;
															#line 556 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >*>(static_cast<StatusFetcherImplActor*>(this)));
															#line 2017 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			loopDepth = 0;
		}
		else
		{
			loopDepth = a_body1cont1loopBody1cont1(loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1break1(int loopDepth) 
	{
		try {
			return a_body1cont9(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1cont1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1cont1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1(int loopDepth) 
	{
															#line 582 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<Void> __when_expr_2 = clusterInterface->onChange();
															#line 581 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state < 0) return a_body1cont1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2046 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1cont1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when1(__when_expr_2.get(), loopDepth); };
															#line 583 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<Void> __when_expr_3 = interfaceTimeout;
															#line 2050 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1cont1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 1)); else return a_body1cont1loopBody1cont1when2(__when_expr_3.get(), loopDepth); };
		static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 3;
															#line 582 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< StatusFetcherImplActor, 2, Void >*>(static_cast<StatusFetcherImplActor*>(this)));
															#line 583 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< StatusFetcherImplActor, 3, Void >*>(static_cast<StatusFetcherImplActor*>(this)));
															#line 2057 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Optional<StatusObject> const& _statusObjCluster,int loopDepth) 
	{
															#line 558 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (_statusObjCluster.present())
															#line 2066 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 559 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObjCluster = _statusObjCluster.get();
															#line 562 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (statusObjCluster.count("fault_tolerance"))
															#line 2072 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 563 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StatusObject::Map& faultToleranceWriteable = statusObjCluster["fault_tolerance"].get_obj();
															#line 564 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StatusObjectReader faultToleranceReader(faultToleranceWriteable);
															#line 565 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				int maxDataLoss, maxAvailLoss;
															#line 566 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (faultToleranceReader.get("max_zone_failures_without_losing_data", maxDataLoss) && faultToleranceReader.get("max_zone_failures_without_losing_availability", maxAvailLoss))
															#line 2082 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				{
															#line 571 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					faultToleranceWriteable["max_zone_failures_without_losing_data"] = std::min(maxDataLoss, coordinatorsFaultTolerance);
															#line 573 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					faultToleranceWriteable["max_zone_failures_without_losing_availability"] = std::min(maxAvailLoss, coordinatorsFaultTolerance);
															#line 2088 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
			}
		}
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont2(Optional<StatusObject> && _statusObjCluster,int loopDepth) 
	{
															#line 558 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (_statusObjCluster.present())
															#line 2100 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 559 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			statusObjCluster = _statusObjCluster.get();
															#line 562 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (statusObjCluster.count("fault_tolerance"))
															#line 2106 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			{
															#line 563 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StatusObject::Map& faultToleranceWriteable = statusObjCluster["fault_tolerance"].get_obj();
															#line 564 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				StatusObjectReader faultToleranceReader(faultToleranceWriteable);
															#line 565 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				int maxDataLoss, maxAvailLoss;
															#line 566 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
				if (faultToleranceReader.get("max_zone_failures_without_losing_data", maxDataLoss) && faultToleranceReader.get("max_zone_failures_without_losing_availability", maxAvailLoss))
															#line 2116 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				{
															#line 571 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					faultToleranceWriteable["max_zone_failures_without_losing_data"] = std::min(maxDataLoss, coordinatorsFaultTolerance);
															#line 573 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
					faultToleranceWriteable["max_zone_failures_without_losing_availability"] = std::min(maxAvailLoss, coordinatorsFaultTolerance);
															#line 2122 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
				}
			}
		}
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<StatusObject> const& _statusObjCluster,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(_statusObjCluster, loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1when1(Optional<StatusObject> && _statusObjCluster,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont2(std::move(_statusObjCluster), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state > 0) static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 0;
		static_cast<StatusFetcherImplActor*>(this)->ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >::remove();

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >*,Optional<StatusObject> const& value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >*,Optional<StatusObject> && value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >*,Error err) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont1loopBody1cont7(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1cont1loopHead1(0);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1loopBody1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when2(Void const& _,int loopDepth) 
	{
															#line 584 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		clientMessages.push_back(makeMessage("no_cluster_controller", "Unable to locate a cluster controller within 2 seconds.  " "Check that there are server processes running."));
															#line 2215 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1cont1loopBody1cont1when2(Void && _,int loopDepth) 
	{
															#line 584 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		clientMessages.push_back(makeMessage("no_cluster_controller", "Unable to locate a cluster controller within 2 seconds.  " "Check that there are server processes running."));
															#line 2224 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		return a_body1cont1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<StatusFetcherImplActor*>(this)->actor_wait_state > 0) static_cast<StatusFetcherImplActor*>(this)->actor_wait_state = 0;
		static_cast<StatusFetcherImplActor*>(this)->ActorCallback< StatusFetcherImplActor, 2, Void >::remove();
		static_cast<StatusFetcherImplActor*>(this)->ActorCallback< StatusFetcherImplActor, 3, Void >::remove();

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< StatusFetcherImplActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 3, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when2(value, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< StatusFetcherImplActor, 3, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont1loopBody1cont1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< StatusFetcherImplActor, 3, Void >*,Error err) 
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose3();
		try {
			a_body1cont1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1cont1Catch1(error, 0);
		} catch (...) {
			a_body1cont1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), 3);

	}
	int a_body1cont11(int loopDepth) 
	{
		try {
			loopDepth = a_body1cont8(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, loopDepth);
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), loopDepth);
		}

		return loopDepth;
	}
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Reference<IClusterConnectionRecord> connRecord;
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Reference<AsyncVar<Optional<ClusterInterface>>> clusterInterface;
															#line 519 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusObject statusObj;
															#line 520 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusObject statusObjClient;
															#line 521 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusArray clientMessages;
															#line 524 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	bool quorum_reachable;
															#line 525 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	int coordinatorsFaultTolerance;
															#line 528 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	int64_t clientTime;
															#line 547 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusObject statusObjCluster;
															#line 552 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Future<Void> interfaceTimeout;
															#line 2359 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
};
// This generated class is to be used only via statusFetcherImpl()
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class StatusFetcherImplActor final : public Actor<StatusObject>, public ActorCallback< StatusFetcherImplActor, 0, StatusObject >, public ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >, public ActorCallback< StatusFetcherImplActor, 2, Void >, public ActorCallback< StatusFetcherImplActor, 3, Void >, public FastAllocated<StatusFetcherImplActor>, public StatusFetcherImplActorState<StatusFetcherImplActor> {
															#line 2364 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
	using FastAllocated<StatusFetcherImplActor>::operator new;
	using FastAllocated<StatusFetcherImplActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<StatusObject>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< StatusFetcherImplActor, 0, StatusObject >;
friend struct ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >;
friend struct ActorCallback< StatusFetcherImplActor, 2, Void >;
friend struct ActorCallback< StatusFetcherImplActor, 3, Void >;
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	StatusFetcherImplActor(Reference<IClusterConnectionRecord> const& connRecord,Reference<AsyncVar<Optional<ClusterInterface>>> const& clusterInterface) 
															#line 2378 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		 : Actor<StatusObject>(),
		   StatusFetcherImplActorState<StatusFetcherImplActor>(connRecord, clusterInterface)
	{
		fdb_probe_actor_enter("statusFetcherImpl", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("statusFetcherImpl");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("statusFetcherImpl", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< StatusFetcherImplActor, 0, StatusObject >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< StatusFetcherImplActor, 1, Optional<StatusObject> >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< StatusFetcherImplActor, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
[[nodiscard]] Future<StatusObject> statusFetcherImpl( Reference<IClusterConnectionRecord> const& connRecord, Reference<AsyncVar<Optional<ClusterInterface>>> const& clusterInterface ) {
															#line 514 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	return Future<StatusObject>(new StatusFetcherImplActor(connRecord, clusterInterface));
															#line 2408 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
}

#line 619 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"

															#line 2413 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
namespace {
// This generated class is to be used only via timeoutMonitorLeader()
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
template <class TimeoutMonitorLeaderActor>
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class TimeoutMonitorLeaderActorState {
															#line 2420 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	TimeoutMonitorLeaderActorState(Database const& db) 
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		 : db(db),
															#line 621 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		   leadMon(monitorLeader<ClusterInterface>(db->getConnectionRecord(), db->statusClusterInterface))
															#line 2429 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
	{
		fdb_probe_actor_create("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this));

	}
	~TimeoutMonitorLeaderActorState() 
	{
		fdb_probe_actor_destroy("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 622 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			;
															#line 2444 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
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
		this->~TimeoutMonitorLeaderActorState();
		static_cast<TimeoutMonitorLeaderActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 623 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		StrictFuture<Void> __when_expr_0 = delay(CLIENT_KNOBS->STATUS_IDLE_TIMEOUT + 0.00001 + db->lastStatusFetch - now());
															#line 623 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (static_cast<TimeoutMonitorLeaderActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2476 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
		static_cast<TimeoutMonitorLeaderActor*>(this)->actor_wait_state = 1;
															#line 623 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TimeoutMonitorLeaderActor, 0, Void >*>(static_cast<TimeoutMonitorLeaderActor*>(this)));
															#line 2481 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void const& _,int loopDepth) 
	{
															#line 624 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (now() - db->lastStatusFetch > CLIENT_KNOBS->STATUS_IDLE_TIMEOUT)
															#line 2490 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 625 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			db->statusClusterInterface = Reference<AsyncVar<Optional<ClusterInterface>>>();
															#line 626 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (!static_cast<TimeoutMonitorLeaderActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TimeoutMonitorLeaderActorState(); static_cast<TimeoutMonitorLeaderActor*>(this)->destroy(); return 0; }
															#line 2496 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			new (&static_cast<TimeoutMonitorLeaderActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TimeoutMonitorLeaderActorState();
			static_cast<TimeoutMonitorLeaderActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont1(Void && _,int loopDepth) 
	{
															#line 624 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
		if (now() - db->lastStatusFetch > CLIENT_KNOBS->STATUS_IDLE_TIMEOUT)
															#line 2510 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		{
															#line 625 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			db->statusClusterInterface = Reference<AsyncVar<Optional<ClusterInterface>>>();
															#line 626 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
			if (!static_cast<TimeoutMonitorLeaderActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TimeoutMonitorLeaderActorState(); static_cast<TimeoutMonitorLeaderActor*>(this)->destroy(); return 0; }
															#line 2516 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
			new (&static_cast<TimeoutMonitorLeaderActor*>(this)->SAV< Void >::value()) Void(Void());
			this->~TimeoutMonitorLeaderActorState();
			static_cast<TimeoutMonitorLeaderActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TimeoutMonitorLeaderActor*>(this)->actor_wait_state > 0) static_cast<TimeoutMonitorLeaderActor*>(this)->actor_wait_state = 0;
		static_cast<TimeoutMonitorLeaderActor*>(this)->ActorCallback< TimeoutMonitorLeaderActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TimeoutMonitorLeaderActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TimeoutMonitorLeaderActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TimeoutMonitorLeaderActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), 0);

	}
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Database db;
															#line 621 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	Future<Void> leadMon;
															#line 2593 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
};
// This generated class is to be used only via timeoutMonitorLeader()
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
class TimeoutMonitorLeaderActor final : public Actor<Void>, public ActorCallback< TimeoutMonitorLeaderActor, 0, Void >, public FastAllocated<TimeoutMonitorLeaderActor>, public TimeoutMonitorLeaderActorState<TimeoutMonitorLeaderActor> {
															#line 2598 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
public:
	using FastAllocated<TimeoutMonitorLeaderActor>::operator new;
	using FastAllocated<TimeoutMonitorLeaderActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TimeoutMonitorLeaderActor, 0, Void >;
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	TimeoutMonitorLeaderActor(Database const& db) 
															#line 2609 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
		 : Actor<Void>(),
		   TimeoutMonitorLeaderActorState<TimeoutMonitorLeaderActor>(db)
	{
		fdb_probe_actor_enter("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("timeoutMonitorLeader");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("timeoutMonitorLeader", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TimeoutMonitorLeaderActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
[[nodiscard]] Future<Void> timeoutMonitorLeader( Database const& db ) {
															#line 620 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"
	return Future<Void>(new TimeoutMonitorLeaderActor(db));
															#line 2637 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.g.cpp"
}

#line 630 "/usr/src/libfdb_c/fdbclient/StatusClient.actor.cpp"

Future<StatusObject> StatusClient::statusFetcher(Database db) {
	db->lastStatusFetch = now();
	if (!db->statusClusterInterface) {
		db->statusClusterInterface = makeReference<AsyncVar<Optional<ClusterInterface>>>();
		db->statusLeaderMon = timeoutMonitorLeader(db);
	}

	return statusFetcherImpl(db->getConnectionRecord(), db->statusClusterInterface);
}
