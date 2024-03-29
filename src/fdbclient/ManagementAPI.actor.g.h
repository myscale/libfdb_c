#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
/*
 * ManagementAPI.actor.h
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
#if defined(NO_INTELLISENSE) && !defined(FDBCLIENT_MANAGEMENT_API_ACTOR_G_H)
#define FDBCLIENT_MANAGEMENT_API_ACTOR_G_H
#include "fdbclient/ManagementAPI.actor.g.h"
#elif !defined(FDBCLIENT_MANAGEMENT_API_ACTOR_H)
#define FDBCLIENT_MANAGEMENT_API_ACTOR_H

/* This file defines "management" interfaces for configuration, coordination changes, and
the inclusion and exclusion of servers. It is used to implement fdbcli management commands
and by test workloads that simulate such. It isn't exposed to C clients or anywhere outside
our code base and doesn't need to be versioned. It doesn't do anything you can't do with the
standard API and some knowledge of the contents of the system key space.
*/

#include <string>
#include <map>
#include "fdbclient/GenericManagementAPI.actor.h"
#include "fdbclient/NativeAPI.actor.h"
#include "fdbclient/ReadYourWrites.h"
#include "fdbclient/DatabaseConfiguration.h"
#include "fdbclient/MonitorLeader.h"
#include "flow/actorcompiler.h" // has to be last include

															#line 46 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<DatabaseConfiguration> getDatabaseConfiguration( Database const& cx );

#line 45 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 50 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> waitForFullReplication( Database const& cx );

#line 46 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

struct IQuorumChange : ReferenceCounted<IQuorumChange> {
	virtual ~IQuorumChange() {}
	virtual Future<std::vector<NetworkAddress>> getDesiredCoordinators(Transaction* tr,
	                                                                   std::vector<NetworkAddress> oldCoordinators,
	                                                                   Reference<IClusterConnectionRecord>,
	                                                                   CoordinatorsResult&) = 0;
	virtual std::string getDesiredClusterKeyName() const { return std::string(); }
};

// Change to use the given set of coordination servers
															#line 65 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Optional<CoordinatorsResult>> changeQuorumChecker( Transaction* const& tr, ClusterConnectionString* const& conn, std::string const& newName );

#line 60 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 69 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<CoordinatorsResult> changeQuorum( Database const& cx, Reference<IQuorumChange> const& change );

#line 61 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
Reference<IQuorumChange> autoQuorumChange(int desired = -1);
Reference<IQuorumChange> nameQuorumChange(std::string const& name, Reference<IQuorumChange> const& other);

// Exclude the given set of servers from use as state servers.  Returns as soon as the change is durable, without
// necessarily waiting for the servers to be evacuated.  A NetworkAddress with a port of 0 means all servers on the
// given IP.
															#line 79 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> excludeServers( Database const& cx, std::vector<AddressExclusion> const& servers, bool const& failed = false );

#line 68 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 83 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> excludeServers( Transaction* const& tr, std::vector<AddressExclusion> const& servers, bool const& failed = false );

#line 69 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Exclude the servers matching the given set of localities from use as state servers.  Returns as soon as the change
// is durable, without necessarily waiting for the servers to be evacuated.
															#line 90 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> excludeLocalities( Database const& cx, std::unordered_set<std::string> const& localities, bool const& failed = false );

#line 73 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 94 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> excludeLocalities( Transaction* const& tr, std::unordered_set<std::string> const& localities, bool const& failed = false );

#line 74 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Remove the given servers from the exclusion list.  A NetworkAddress with a port of 0 means all servers on the given
// IP.  A NetworkAddress() means all servers (don't exclude anything)
															#line 101 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> includeServers( Database const& cx, std::vector<AddressExclusion> const& servers, bool const& failed = false );

#line 78 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Remove the given localities from the exclusion list.
															#line 107 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> includeLocalities( Database const& cx, std::vector<std::string> const& localities, bool const& failed = false, bool const& includeAll = false );

#line 84 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Set the process class of processes with the given address.  A NetworkAddress with a port of 0 means all servers on
// the given IP.
															#line 114 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> setClass( Database const& cx, AddressExclusion const& server, ProcessClass const& processClass );

#line 88 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of excluded servers including both "exclude" and "failed".
															#line 120 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<AddressExclusion>> getAllExcludedServers( Database const& cx );

#line 91 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<AddressExclusion>> getAllExcludedServers( Transaction* const& tr );

#line 92 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of excluded servers.
															#line 130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<AddressExclusion>> getExcludedServerList( Transaction* const& tr );

#line 95 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of failed servers.
															#line 136 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<AddressExclusion>> getExcludedFailedServerList( Transaction* const& tr );

#line 98 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of excluded localities
															#line 142 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<std::string>> getAllExcludedLocalities( Database const& cx );

#line 101 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 146 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<std::string>> getAllExcludedLocalities( Transaction* const& tr );

#line 102 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of excluded localities.
															#line 152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<std::string>> getExcludedLocalityList( Transaction* const& tr );

#line 105 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Get the current list of failed localities.
															#line 158 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<std::string>> getExcludedFailedLocalityList( Transaction* const& tr );

#line 108 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

std::set<AddressExclusion> getAddressesByLocality(const std::vector<ProcessData>& workers, const std::string& locality);

// Check for the given, previously excluded servers to be evacuated (no longer used for state).  If waitForExclusion is
// true, this actor returns once it is safe to shut down all such machines without impacting fault tolerance, until and
// unless any of them are explicitly included with includeServers()
															#line 168 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::set<NetworkAddress>> checkForExcludingServers( Database const& cx, std::vector<AddressExclusion> const& servers, bool const& waitForAllExcluded );

#line 117 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 172 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<bool> checkForExcludingServersTxActor( ReadYourWritesTransaction* const& tr, std::set<AddressExclusion>* const& exclusions, std::set<NetworkAddress>* const& inProgressExclusion );

#line 120 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Gets a list of all workers in the cluster (excluding testers)
															#line 178 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Database const& cx );

#line 123 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 182 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<std::vector<ProcessData>> getWorkers( Transaction* const& tr );

#line 124 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 187 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> timeKeeperSetDisable( Database const& cx );

#line 126 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 192 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> lockDatabase( Transaction* const& tr, UID const& id );

#line 128 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 196 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> lockDatabase( Reference<ReadYourWritesTransaction> const& tr, UID const& id );

#line 129 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 200 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> lockDatabase( Database const& cx, UID const& id );

#line 130 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 205 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> unlockDatabase( Transaction* const& tr, UID const& id );

#line 132 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 209 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> unlockDatabase( Reference<ReadYourWritesTransaction> const& tr, UID const& id );

#line 133 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 213 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> unlockDatabase( Database const& cx, UID const& id );

#line 134 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 218 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> checkDatabaseLock( Transaction* const& tr, UID const& id );

#line 136 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 222 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> checkDatabaseLock( Reference<ReadYourWritesTransaction> const& tr, UID const& id );

#line 137 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 227 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> updateChangeFeed( Transaction* const& tr, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range = KeyRange() );

#line 139 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 231 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> updateChangeFeed( Reference<ReadYourWritesTransaction> const& tr, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range = KeyRange() );

#line 143 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 235 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> updateChangeFeed( Database const& cx, Key const& rangeID, ChangeFeedStatus const& status, KeyRange const& range = KeyRange() );

#line 144 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 240 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> advanceVersion( Database const& cx, Version const& v );

#line 146 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 245 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<int> setDDMode( Database const& cx, int const& mode );

#line 148 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 250 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> forceRecovery( Reference<IClusterConnectionRecord> const& clusterFile, Standalone<StringRef> const& dcId );

#line 150 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 255 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> printHealthyZone( Database const& cx );

#line 152 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 259 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> setDDIgnoreRebalanceSwitch( Database const& cx, bool const& ignoreRebalance );

#line 153 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 263 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<bool> clearHealthyZone( Database const& cx, bool const& printWarning = false, bool const& clearSSFailureZoneString = false );

#line 154 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"
															#line 267 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<bool> setHealthyZone( Database const& cx, StringRef const& zoneId, double const& seconds, bool const& printWarning = false );

#line 155 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

															#line 272 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> waitForPrimaryDC( Database const& cx, StringRef const& dcId );

#line 157 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

// Gets the cluster connection string
															#line 278 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Optional<ClusterConnectionString>> getConnectionString( Database const& cx );

#line 160 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

void schemaCoverage(std::string const& spath, bool covered = true);
bool schemaMatch(json_spirit::mValue const& schema,
                 json_spirit::mValue const& result,
                 std::string& errorStr,
                 Severity sev = SevError,
                 bool checkCoverage = false,
                 std::string path = std::string(),
                 std::string schema_path = std::string());

// execute payload in 'snapCmd' on all the coordinators, TLogs and
// storage nodes
															#line 294 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.g.h"
[[nodiscard]] Future<Void> mgmtSnapCreate( Database const& cx, Standalone<StringRef> const& snapCmd, UID const& snapUID );

#line 173 "/usr/src/libfdb_c/fdbclient/ManagementAPI.actor.h"

#include "flow/unactorcompiler.h"
#endif
