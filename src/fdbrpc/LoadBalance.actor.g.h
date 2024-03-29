#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
/*
 * LoadBalance.actor.h
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

// When actually compiled (NO_INTELLISENSE), include the generated version of this file.  In intellisense use the source
// version.
#if defined(NO_INTELLISENSE) && !defined(FLOW_LOADBALANCE_ACTOR_G_H)
#define FLOW_LOADBALANCE_ACTOR_G_H
#include "fdbrpc/LoadBalance.actor.g.h"
#elif !defined(FLOW_LOADBALANCE_ACTOR_H)
#define FLOW_LOADBALANCE_ACTOR_H

#include "flow/BooleanParam.h"
#include "flow/flow.h"
#include "flow/Knobs.h"

#include "fdbrpc/FailureMonitor.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/Locality.h"
#include "fdbrpc/QueueModel.h"
#include "fdbrpc/MultiInterface.h"
#include "fdbrpc/simulator.h" // for checking tss simulation mode
#include "fdbrpc/TSSComparison.h"
#include "flow/actorcompiler.h" // This must be the last #include.

															#line 46 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
[[nodiscard]] Future<Void> allAlternativesFailedDelay( Future<Void> const& okFuture );

#line 45 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"

struct ModelHolder : NonCopyable, public ReferenceCounted<ModelHolder> {
	QueueModel* model;
	bool released;
	double startTime;
	double delta;
	uint64_t token;

	ModelHolder(QueueModel* model, uint64_t token) : model(model), released(false), startTime(now()), token(token) {
		if (model) {
			delta = model->addRequest(token);
		}
	}

	void release(bool clean, bool futureVersion, double penalty, bool measureLatency = true) {
		if (model && !released) {
			released = true;
			double latency = (clean || measureLatency) ? now() - startTime : 0.0;
			model->endRequest(token, latency, penalty, delta, clean, futureVersion);
		}
	}

	~ModelHolder() { release(false, false, -1.0, false); }
};

// Subclasses must initialize all members in their default constructors
// Subclasses must serialize all members
struct LoadBalancedReply {
	double penalty;
	Optional<Error> error;
	LoadBalancedReply() : penalty(1.0) {}
};

Optional<LoadBalancedReply> getLoadBalancedReply(const LoadBalancedReply* reply);
Optional<LoadBalancedReply> getLoadBalancedReply(const void*);

															#line 86 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via tssComparison()
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi, class TssComparisonActor>
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class TssComparisonActorState {
															#line 93 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TssComparisonActorState(Req const& req,Future<ErrorOr<Resp>> const& fSource,Future<ErrorOr<Resp>> const& fTss,TSSEndpointData const& tssData,uint64_t const& srcEndpointId,Reference<MultiInterface<Multi>> const& ssTeam,RequestStream<Req> Interface::* const& channel) 
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		 : req(req),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   fSource(fSource),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   fTss(fTss),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   tssData(tssData),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   srcEndpointId(srcEndpointId),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   ssTeam(ssTeam),
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 89 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   startTime(now()),
															#line 90 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   fTssWithTimeout(timeout(fTss, FLOW_KNOBS->LOAD_BALANCE_TSS_TIMEOUT)),
															#line 91 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   finished(0),
															#line 92 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   srcEndTime(),
															#line 93 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   tssEndTime(),
															#line 95 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   srcErrorCode(error_code_success),
															#line 96 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   tssErrorCode(error_code_success),
															#line 97 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   src(),
															#line 98 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   tss()
															#line 130 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("tssComparison", reinterpret_cast<unsigned long>(this));

	}
	~TssComparisonActorState() 
	{
		fdb_probe_actor_destroy("tssComparison", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 100 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			;
															#line 145 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
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
		this->~TssComparisonActorState();
		static_cast<TssComparisonActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 120 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		++tssData.metrics->requests;
															#line 122 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (src.isError())
															#line 170 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 123 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			srcErrorCode = src.getError().code();
															#line 124 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			tssData.metrics->ssError(srcErrorCode);
															#line 176 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 126 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (!tss.present())
															#line 180 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 127 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			++tssData.metrics->tssTimeouts;
															#line 184 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		else
		{
															#line 128 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (tss.get().isError())
															#line 190 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 129 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				tssErrorCode = tss.get().getError().code();
															#line 130 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->tssError(tssErrorCode);
															#line 196 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
		}
															#line 132 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (!src.isError() && tss.present() && !tss.get().isError())
															#line 201 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 133 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			Optional<LoadBalancedReply> srcLB = getLoadBalancedReply(&src.get());
															#line 134 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			Optional<LoadBalancedReply> tssLB = getLoadBalancedReply(&tss.get().get());
															#line 135 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ASSERT(srcLB.present() == tssLB.present());
															#line 139 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!srcLB.present() || (!srcLB.get().error.present() && !tssLB.get().error.present()))
															#line 211 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 142 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->recordLatency(req, srcEndTime - startTime, tssEndTime - startTime);
															#line 144 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (!TSS_doCompare(src.get(), tss.get().get()))
															#line 217 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 145 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					TEST(true);
															#line 146 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					mismatchEvent = TraceEvent((g_network->isSimulated() && g_simulator.tssMode == ISimulator::TSSMode::EnabledDropMutations) ? SevWarnAlways : SevError, TSS_mismatchTraceName(req));
															#line 151 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					mismatchEvent.setMaxEventLength(FLOW_KNOBS->TSS_LARGE_TRACE_SIZE);
															#line 152 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					mismatchEvent.detail("TSSID", tssData.tssId);
															#line 154 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_VERIFY_SS && ssTeam->size() > 1)
															#line 229 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 155 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						TEST(true);
															#line 159 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						restOfTeamFutures = std::vector<Future<ErrorOr<Resp>>>();
															#line 160 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						restOfTeamFutures.reserve(ssTeam->size() - 1);
															#line 161 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						for(int i = 0;i < ssTeam->size();i++) {
															#line 162 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							RequestStream<Req> const* si = &ssTeam->get(i, channel);
															#line 163 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							if (si->getEndpoint().token.first() != srcEndpointId)
															#line 243 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 165 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								resetReply(req);
															#line 166 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								restOfTeamFutures.push_back(si->tryGetReply(req));
															#line 249 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							}
						}
															#line 170 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						StrictFuture<Void> __when_expr_2 = waitForAllReady(restOfTeamFutures);
															#line 170 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (static_cast<TssComparisonActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 256 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), loopDepth); else return a_body1cont1when1(__when_expr_2.get(), loopDepth); };
						static_cast<TssComparisonActor*>(this)->actor_wait_state = 2;
															#line 170 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 2, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 261 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						loopDepth = 0;
					}
					else
					{
						loopDepth = a_body1cont9(loopDepth);
					}
				}
				else
				{
					loopDepth = a_body1cont8(loopDepth);
				}
			}
			else
			{
															#line 222 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (tssLB.present() && tssLB.get().error.present())
															#line 278 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 223 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					tssErrorCode = tssLB.get().error.get().code();
															#line 224 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					tssData.metrics->tssError(tssErrorCode);
															#line 284 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 225 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (srcLB.present() && srcLB.get().error.present())
															#line 290 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 226 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						srcErrorCode = srcLB.get().error.get().code();
															#line 227 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						tssData.metrics->ssError(srcErrorCode);
															#line 296 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
				}
				loopDepth = a_body1cont7(loopDepth);
			}
		}
		else
		{
			loopDepth = a_body1cont2(loopDepth);
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
															#line 102 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_0 = store(src, fSource);
															#line 101 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 322 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
															#line 110 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_1 = store(tss, fTssWithTimeout);
															#line 326 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
		static_cast<TssComparisonActor*>(this)->actor_wait_state = 1;
															#line 102 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 0, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 110 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< TssComparisonActor, 1, Void >*>(static_cast<TssComparisonActor*>(this)));
															#line 333 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

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
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
															#line 103 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		srcEndTime = now();
															#line 104 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		fSource = Never();
															#line 105 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 106 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 367 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
															#line 103 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		srcEndTime = now();
															#line 104 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		fSource = Never();
															#line 105 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 106 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 385 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
															#line 111 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		tssEndTime = now();
															#line 112 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		fTssWithTimeout = Never();
															#line 113 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 114 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 403 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
															#line 111 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		tssEndTime = now();
															#line 112 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		fTssWithTimeout = Never();
															#line 113 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		finished++;
															#line 114 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (finished == 2)
															#line 421 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
			return a_body1break1(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state > 0) static_cast<TssComparisonActor*>(this)->actor_wait_state = 0;
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 0, Void >::remove();
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1cont2(int loopDepth) 
	{
															#line 231 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (srcErrorCode != error_code_success && tssErrorCode != error_code_success && srcErrorCode != tssErrorCode)
															#line 530 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 233 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			TraceEvent("TSSErrorMismatch") .suppressFor(1.0) .detail("TSSID", tssData.tssId) .detail("SSError", srcErrorCode) .detail("TSSError", tssErrorCode);
															#line 534 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 240 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (!static_cast<TssComparisonActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~TssComparisonActorState(); static_cast<TssComparisonActor*>(this)->destroy(); return 0; }
															#line 538 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		new (&static_cast<TssComparisonActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~TssComparisonActorState();
		static_cast<TssComparisonActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1cont7(int loopDepth) 
	{
		loopDepth = a_body1cont2(loopDepth);

		return loopDepth;
	}
	int a_body1cont8(int loopDepth) 
	{
		loopDepth = a_body1cont7(loopDepth);

		return loopDepth;
	}
	int a_body1cont9(int loopDepth) 
	{
															#line 197 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (tssData.metrics->shouldRecordDetailedMismatch())
															#line 562 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 198 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			TSS_traceMismatch(mismatchEvent, req, src.get(), tss.get().get());
															#line 200 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			TEST(FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL);
															#line 201 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			TEST(!FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL);
															#line 204 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!FLOW_KNOBS->LOAD_BALANCE_TSS_MISMATCH_TRACE_FULL)
															#line 572 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 205 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				mismatchEvent.disable();
															#line 206 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				UID mismatchUID = deterministicRandom()->randomUniqueID();
															#line 207 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				tssData.metrics->recordDetailedMismatchData(mismatchUID, mismatchEvent.getFields().toString());
															#line 210 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				TraceEvent summaryEvent((g_network->isSimulated() && g_simulator.tssMode == ISimulator::TSSMode::EnabledDropMutations) ? SevWarnAlways : SevError, TSS_mismatchTraceName(req));
															#line 215 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				summaryEvent.detail("TSSID", tssData.tssId).detail("MismatchId", mismatchUID);
															#line 584 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
		}
		else
		{
															#line 219 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			mismatchEvent.disable();
															#line 591 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1cont8(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void const& _,int loopDepth) 
	{
															#line 172 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numError = 0;
															#line 173 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchSS = 0;
															#line 174 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchTSS = 0;
															#line 175 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchNeither = 0;
															#line 176 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		for( Future<ErrorOr<Resp>> f : restOfTeamFutures ) {
															#line 177 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!f.canGet() || f.get().isError())
															#line 611 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 178 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				numError++;
															#line 615 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 180 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				Optional<LoadBalancedReply> fLB = getLoadBalancedReply(&f.get().get());
															#line 181 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (fLB.present() && fLB.get().error.present())
															#line 623 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 182 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					numError++;
															#line 627 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 183 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (TSS_doCompare(src.get(), f.get().get()))
															#line 633 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 184 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						numMatchSS++;
															#line 637 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 185 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (TSS_doCompare(tss.get().get(), f.get().get()))
															#line 643 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 186 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							numMatchTSS++;
															#line 647 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
						else
						{
															#line 188 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							numMatchNeither++;
															#line 653 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
					}
				}
			}
		}
															#line 192 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		mismatchEvent.detail("TeamCheckErrors", numError) .detail("TeamCheckMatchSS", numMatchSS) .detail("TeamCheckMatchTSS", numMatchTSS) .detail("TeamCheckMatchNeither", numMatchNeither);
															#line 661 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont10(Void && _,int loopDepth) 
	{
															#line 172 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numError = 0;
															#line 173 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchSS = 0;
															#line 174 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchTSS = 0;
															#line 175 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		int numMatchNeither = 0;
															#line 176 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		for( Future<ErrorOr<Resp>> f : restOfTeamFutures ) {
															#line 177 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!f.canGet() || f.get().isError())
															#line 680 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 178 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				numError++;
															#line 684 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 180 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				Optional<LoadBalancedReply> fLB = getLoadBalancedReply(&f.get().get());
															#line 181 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (fLB.present() && fLB.get().error.present())
															#line 692 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 182 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					numError++;
															#line 696 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
				else
				{
															#line 183 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (TSS_doCompare(src.get(), f.get().get()))
															#line 702 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 184 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						numMatchSS++;
															#line 706 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 185 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (TSS_doCompare(tss.get().get(), f.get().get()))
															#line 712 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 186 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							numMatchTSS++;
															#line 716 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
						else
						{
															#line 188 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							numMatchNeither++;
															#line 722 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
					}
				}
			}
		}
															#line 192 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		mismatchEvent.detail("TeamCheckErrors", numError) .detail("TeamCheckMatchSS", numMatchSS) .detail("TeamCheckMatchTSS", numMatchTSS) .detail("TeamCheckMatchNeither", numMatchNeither);
															#line 730 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1cont9(loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont10(_, loopDepth);

		return loopDepth;
	}
	int a_body1cont1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont10(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<TssComparisonActor*>(this)->actor_wait_state > 0) static_cast<TssComparisonActor*>(this)->actor_wait_state = 0;
		static_cast<TssComparisonActor*>(this)->ActorCallback< TssComparisonActor, 2, Void >::remove();

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 2, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< TssComparisonActor, 2, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1cont1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< TssComparisonActor, 2, Void >*,Error err) 
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Req req;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Future<ErrorOr<Resp>> fSource;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Future<ErrorOr<Resp>> fTss;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TSSEndpointData tssData;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	uint64_t srcEndpointId;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Reference<MultiInterface<Multi>> ssTeam;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestStream<Req> Interface::* channel;
															#line 89 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double startTime;
															#line 90 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Future<Optional<ErrorOr<Resp>>> fTssWithTimeout;
															#line 91 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int finished;
															#line 92 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double srcEndTime;
															#line 93 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double tssEndTime;
															#line 95 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int srcErrorCode;
															#line 96 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int tssErrorCode;
															#line 97 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	ErrorOr<Resp> src;
															#line 98 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Optional<ErrorOr<Resp>> tss;
															#line 146 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TraceEvent mismatchEvent;
															#line 159 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	std::vector<Future<ErrorOr<Resp>>> restOfTeamFutures;
															#line 834 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via tssComparison()
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi>
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class TssComparisonActor final : public Actor<Void>, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 0, Void >, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 1, Void >, public ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 2, Void >, public FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi>>, public TssComparisonActorState<Req, Resp, Interface, Multi, TssComparisonActor<Req, Resp, Interface, Multi>> {
															#line 841 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi>>::operator new;
	using FastAllocated<TssComparisonActor<Req, Resp, Interface, Multi>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 0, Void >;
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 1, Void >;
friend struct ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 2, Void >;
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TssComparisonActor(Req const& req,Future<ErrorOr<Resp>> const& fSource,Future<ErrorOr<Resp>> const& fTss,TSSEndpointData const& tssData,uint64_t const& srcEndpointId,Reference<MultiInterface<Multi>> const& ssTeam,RequestStream<Req> Interface::* const& channel) 
															#line 854 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<Void>(),
		   TssComparisonActorState<Req, Resp, Interface, Multi, TssComparisonActor<Req, Resp, Interface, Multi>>(req, fSource, fTss, tssData, srcEndpointId, ssTeam, channel)
	{
		fdb_probe_actor_enter("tssComparison", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("tssComparison");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("tssComparison", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< TssComparisonActor<Req, Resp, Interface, Multi>, 2, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Req, class Resp, class Interface, class Multi>
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<Void> tssComparison( Req const& req, Future<ErrorOr<Resp>> const& fSource, Future<ErrorOr<Resp>> const& fTss, TSSEndpointData const& tssData, uint64_t const& srcEndpointId, Reference<MultiInterface<Multi>> const& ssTeam, RequestStream<Req> Interface::* const& channel ) {
															#line 81 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	return Future<Void>(new TssComparisonActor<Req, Resp, Interface, Multi>(req, fSource, fTss, tssData, srcEndpointId, ssTeam, channel));
															#line 885 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
}

#line 242 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"

FDB_DECLARE_BOOLEAN_PARAM(AtMostOnce);
FDB_DECLARE_BOOLEAN_PARAM(TriedAllOptions);

// Stores state for a request made by the load balancer
template <class Request, class Interface, class Multi>
struct RequestData : NonCopyable {
	typedef ErrorOr<REPLY_TYPE(Request)> Reply;

	Future<Reply> response;
	Reference<ModelHolder> modelHolder;
	TriedAllOptions triedAllOptions{ false };

	bool requestStarted = false; // true once the request has been sent to an alternative
	bool requestProcessed = false; // true once a response has been received and handled by checkAndProcessResult

	// Whether or not the response future is valid
	// This is true once setupRequest is called, even though at that point the response is Never().
	bool isValid() { return response.isValid(); }

	static void maybeDuplicateTSSRequest(RequestStream<Request> const* stream,
	                                     Request& request,
	                                     QueueModel* model,
	                                     Future<Reply> ssResponse,
	                                     Reference<MultiInterface<Multi>> alternatives,
	                                     RequestStream<Request> Interface::*channel) {
		if (model) {
			// Send parallel request to TSS pair, if it exists
			Optional<TSSEndpointData> tssData = model->getTssData(stream->getEndpoint().token.first());

			if (tssData.present()) {
				TEST(true); // duplicating request to TSS
				resetReply(request);
				// FIXME: optimize to avoid creating new netNotifiedQueue for each message
				RequestStream<Request> tssRequestStream(tssData.get().endpoint);
				Future<ErrorOr<REPLY_TYPE(Request)>> fTssResult = tssRequestStream.tryGetReply(request);
				model->addActor.send(tssComparison(request,
				                                   ssResponse,
				                                   fTssResult,
				                                   tssData.get(),
				                                   stream->getEndpoint().token.first(),
				                                   alternatives,
				                                   channel));
			}
		}
	}

	// Initializes the request state and starts it, possibly after a backoff delay
	void startRequest(
	    double backoff,
	    TriedAllOptions triedAllOptions,
	    RequestStream<Request> const* stream,
	    Request& request,
	    QueueModel* model,
	    Reference<MultiInterface<Multi>> alternatives, // alternatives and channel passed through for TSS check
	    RequestStream<Request> Interface::*channel) {
		modelHolder = Reference<ModelHolder>();
		requestStarted = false;

		if (backoff > 0) {
			response = mapAsync<Void, std::function<Future<Reply>(Void)>, Reply>(
			    delay(backoff), [this, stream, &request, model, alternatives, channel](Void _) {
				    requestStarted = true;
				    modelHolder = Reference<ModelHolder>(new ModelHolder(model, stream->getEndpoint().token.first()));
				    Future<Reply> resp = stream->tryGetReply(request);
				    maybeDuplicateTSSRequest(stream, request, model, resp, alternatives, channel);
				    return resp;
			    });
		} else {
			requestStarted = true;
			modelHolder = Reference<ModelHolder>(new ModelHolder(model, stream->getEndpoint().token.first()));
			response = stream->tryGetReply(request);
			maybeDuplicateTSSRequest(stream, request, model, response, alternatives, channel);
		}

		requestProcessed = false;
		this->triedAllOptions = triedAllOptions;
	}

	// Implementation of the logic to handle a response.
	// Checks the state of the response, updates the queue model, and returns one of the following outcomes:
	// A return value of true means that the request completed successfully
	// A return value of false means that the request failed but should be retried
	// A return value with an error means that the error should be thrown back to original caller
	static ErrorOr<bool> checkAndProcessResultImpl(Reply const& result,
	                                               Reference<ModelHolder> modelHolder,
	                                               AtMostOnce atMostOnce,
	                                               TriedAllOptions triedAllOptions) {
		ASSERT(modelHolder);

		Optional<LoadBalancedReply> loadBalancedReply;
		if (!result.isError()) {
			loadBalancedReply = getLoadBalancedReply(&result.get());
		}

		int errCode;
		if (loadBalancedReply.present()) {
			errCode = loadBalancedReply.get().error.present() ? loadBalancedReply.get().error.get().code()
			                                                  : error_code_success;
		} else {
			errCode = result.isError() ? result.getError().code() : error_code_success;
		}

		bool maybeDelivered = errCode == error_code_broken_promise || errCode == error_code_request_maybe_delivered;
		bool receivedResponse =
		    loadBalancedReply.present() ? !loadBalancedReply.get().error.present() : result.present();
		receivedResponse = receivedResponse || (!maybeDelivered && errCode != error_code_process_behind);
		bool futureVersion = errCode == error_code_future_version || errCode == error_code_process_behind;

		modelHolder->release(
		    receivedResponse, futureVersion, loadBalancedReply.present() ? loadBalancedReply.get().penalty : -1.0);

		if (errCode == error_code_server_overloaded) {
			return false;
		}

		if (loadBalancedReply.present() && !loadBalancedReply.get().error.present()) {
			return true;
		}

		if (!loadBalancedReply.present() && result.present()) {
			return true;
		}

		if (receivedResponse) {
			return loadBalancedReply.present() ? loadBalancedReply.get().error.get() : result.getError();
		}

		if (atMostOnce && maybeDelivered) {
			return request_maybe_delivered();
		}

		if (triedAllOptions && errCode == error_code_process_behind) {
			return process_behind();
		}

		return false;
	}

	// Checks the state of the response, updates the queue model, and returns one of the following outcomes:
	// A return value of true means that the request completed successfully
	// A return value of false means that the request failed but should be retried
	// In the event of a non-retryable failure, an error is thrown indicating the failure
	bool checkAndProcessResult(AtMostOnce atMostOnce) {
		ASSERT(response.isReady());
		requestProcessed = true;

		ErrorOr<bool> outcome =
		    checkAndProcessResultImpl(response.get(), std::move(modelHolder), atMostOnce, triedAllOptions);

		if (outcome.isError()) {
			throw outcome.getError();
		} else if (!outcome.get()) {
			response = Future<Reply>();
		}

		return outcome.get();
	}

	// Convert this request to a lagging request. Such a request is no longer being waited on, but it still needs to be
	// processed so we can update the queue model.
	void makeLaggingRequest() {
		ASSERT(response.isValid());
		ASSERT(!response.isReady());
		ASSERT(modelHolder);
		ASSERT(modelHolder->model);

		QueueModel* model = modelHolder->model;
		if (model->laggingRequestCount > FLOW_KNOBS->MAX_LAGGING_REQUESTS_OUTSTANDING ||
		    model->laggingRequests.isReady()) {
			model->laggingRequests.cancel();
			model->laggingRequestCount = 0;
			model->addActor = PromiseStream<Future<Void>>();
			model->laggingRequests = actorCollection(model->addActor.getFuture(), &model->laggingRequestCount);
		}

		// We need to process the lagging request in order to update the queue model
		Reference<ModelHolder> holderCapture = std::move(modelHolder);
		auto triedAllOptionsCapture = triedAllOptions;
		Future<Void> updateModel = map(response, [holderCapture, triedAllOptionsCapture](Reply result) {
			checkAndProcessResultImpl(result, holderCapture, AtMostOnce::False, triedAllOptionsCapture);
			return Void();
		});
		model->addActor.send(updateModel);
	}

	~RequestData() {
		// If the request has been started but hasn't completed, mark it as a lagging request
		if (requestStarted && !requestProcessed && modelHolder && modelHolder->model) {
			makeLaggingRequest();
		}
	}
};

// Try to get a reply from one of the alternatives until success, cancellation, or certain errors.
// Load balancing has a budget to race requests to a second alternative if the first request is slow.
// Tries to take into account failMon's information for load balancing and avoiding failed servers.
// If ALL the servers are failed and the list of servers is not fresh, throws an exception to let the caller refresh the
// list of servers.
// When model is set, load balance among alternatives in the same DC aims to balance request queue length on these
// interfaces. If too many interfaces in the same DC are bad, try remote interfaces.
															#line 1090 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via loadBalance()
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, class LoadBalanceActor>
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class LoadBalanceActorState {
															#line 1097 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	LoadBalanceActorState(Reference<MultiInterface<Multi>> const& alternatives,RequestStream<Request> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,QueueModel* const& model = nullptr) 
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		 : alternatives(alternatives),
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   request(request),
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   taskID(taskID),
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   atMostOnce(atMostOnce),
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   model(model),
															#line 453 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   firstRequestData(),
															#line 454 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   secondRequestData(),
															#line 456 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   firstRequestEndpoint(),
															#line 457 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   secondDelay(Never()),
															#line 459 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   requestFinished(),
															#line 460 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   startTime(now()),
															#line 462 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   triedAllOptions(TriedAllOptions::False)
															#line 1128 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("loadBalance", reinterpret_cast<unsigned long>(this));

	}
	~LoadBalanceActorState() 
	{
		fdb_probe_actor_destroy("loadBalance", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 464 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			setReplyPriority(request, taskID);
															#line 465 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!alternatives)
															#line 1145 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 466 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				this->~LoadBalanceActorState();
															#line 1149 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				static_cast<LoadBalanceActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 468 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ASSERT(alternatives->size());
															#line 470 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			bestAlt = deterministicRandom()->randomInt(0, alternatives->countBest());
															#line 471 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			nextAlt = deterministicRandom()->randomInt(0, std::max(alternatives->size() - 1, 1));
															#line 472 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (nextAlt >= bestAlt)
															#line 1161 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 473 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				nextAlt++;
															#line 1165 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 475 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (model)
															#line 1169 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 476 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				double bestMetric = 1e9;
															#line 477 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				double nextMetric = 1e9;
															#line 478 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				double bestTime = 1e9;
															#line 479 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				double nextTime = 1e9;
															#line 480 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				int badServers = 0;
															#line 482 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				for(int i = 0;i < alternatives->size();i++) {
															#line 485 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (badServers < std::min(i, FLOW_KNOBS->LOAD_BALANCE_MAX_BAD_OPTIONS + 1) && i == alternatives->countBest())
															#line 1185 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
						break;
					}
					else
					{
															#line 491 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (badServers == alternatives->countBest() && i == badServers)
															#line 1193 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 492 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							TraceEvent("AllLocalAlternativesFailed") .suppressFor(1.0) .detail("Alternatives", alternatives->description()) .detail("Total", alternatives->size()) .detail("Best", alternatives->countBest());
															#line 1197 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
					}
															#line 499 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					RequestStream<Request> const* thisStream = &alternatives->get(i, channel);
															#line 500 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (!IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed)
															#line 1204 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 501 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						auto const& qd = model->getMeasurement(thisStream->getEndpoint().token.first());
															#line 502 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (now() > qd.failedUntil)
															#line 1210 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 503 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							double thisMetric = qd.smoothOutstanding.smoothTotal();
															#line 504 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							double thisTime = qd.latency;
															#line 505 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							if (FLOW_KNOBS->LOAD_BALANCE_PENALTY_IS_BAD && qd.penalty > 1.001)
															#line 1218 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 509 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								++badServers;
															#line 1222 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							}
															#line 512 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							if (thisMetric < bestMetric)
															#line 1226 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 513 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								if (i != bestAlt)
															#line 1230 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 514 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextAlt = bestAlt;
															#line 515 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextMetric = bestMetric;
															#line 516 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextTime = bestTime;
															#line 1238 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								}
															#line 518 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								bestAlt = i;
															#line 519 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								bestMetric = thisMetric;
															#line 520 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								bestTime = thisTime;
															#line 1246 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							}
							else
							{
															#line 521 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								if (thisMetric < nextMetric)
															#line 1252 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 522 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextAlt = i;
															#line 523 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextMetric = thisMetric;
															#line 524 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextTime = thisTime;
															#line 1260 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								}
							}
						}
						else
						{
															#line 527 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							++badServers;
															#line 1268 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						}
					}
					else
					{
															#line 530 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						++badServers;
															#line 1275 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
				}
															#line 533 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (nextMetric > 1e8)
															#line 1280 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 537 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					for(int i = alternatives->countBest();i < alternatives->size();i++) {
															#line 538 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						RequestStream<Request> const* thisStream = &alternatives->get(i, channel);
															#line 539 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						if (!IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed)
															#line 1288 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
						{
															#line 540 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							auto const& qd = model->getMeasurement(thisStream->getEndpoint().token.first());
															#line 541 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
							if (now() > qd.failedUntil)
															#line 1294 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
							{
															#line 542 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								double thisMetric = qd.smoothOutstanding.smoothTotal();
															#line 543 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								double thisTime = qd.latency;
															#line 545 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
								if (thisMetric < nextMetric)
															#line 1302 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								{
															#line 546 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextAlt = i;
															#line 547 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextMetric = thisMetric;
															#line 548 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
									nextTime = thisTime;
															#line 1310 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
								}
							}
						}
					}
				}
															#line 555 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (nextTime < 1e9)
															#line 1318 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 557 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (bestTime > FLOW_KNOBS->INSTANT_SECOND_REQUEST_MULTIPLIER * (model->secondMultiplier * (nextTime) + FLOW_KNOBS->BASE_SECOND_REQUEST_TIME))
															#line 1322 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 559 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						secondDelay = Void();
															#line 1326 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
					else
					{
															#line 561 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						secondDelay = delay(model->secondMultiplier * nextTime + FLOW_KNOBS->BASE_SECOND_REQUEST_TIME);
															#line 1332 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
				}
				else
				{
															#line 564 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					secondDelay = Never();
															#line 1339 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 568 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			startAlt = nextAlt;
															#line 569 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			startDistance = (bestAlt + alternatives->size() - startAlt) % alternatives->size();
															#line 571 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			numAttempts = 0;
															#line 572 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = 0;
															#line 574 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			;
															#line 1352 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
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
		this->~LoadBalanceActorState();
		static_cast<LoadBalanceActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 575 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (now() - startTime > (g_network->isSimulated() ? 30.0 : 600.0))
															#line 1382 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 576 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			TraceEvent ev(g_network->isSimulated() ? SevWarn : SevWarnAlways, "LoadBalanceTooLong");
															#line 577 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ev.suppressFor(1.0);
															#line 578 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ev.detail("Duration", now() - startTime);
															#line 579 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ev.detail("NumAttempts", numAttempts);
															#line 580 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ev.detail("Backoff", backoff);
															#line 581 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ev.detail("TriedAllOptions", triedAllOptions);
															#line 582 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (ev.isEnabled())
															#line 1398 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 583 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				ev.log();
															#line 584 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 585 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					RequestStream<Request> const* thisStream = &alternatives->get(alternativeNum, channel);
															#line 586 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					TraceEvent(SevWarn, "LoadBalanceTooLongEndpoint") .detail("Addr", thisStream->getEndpoint().getPrimaryAddress()) .detail("Token", thisStream->getEndpoint().token) .detail("Failed", IFailureMonitor::failureMonitor().getState(thisStream->getEndpoint()).failed);
															#line 1408 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
			}
		}
															#line 597 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		stream = nullptr;
															#line 598 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		distance = LBDistance::Type();
															#line 599 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 600 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			int useAlt = nextAlt;
															#line 601 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 1422 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 602 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				useAlt = bestAlt;
															#line 1426 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 603 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if ((nextAlt + alternatives->size() - startAlt) % alternatives->size() <= startDistance)
															#line 1432 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 604 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					useAlt = (nextAlt + alternatives->size() - 1) % alternatives->size();
															#line 1436 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 606 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			stream = &alternatives->get(useAlt, channel);
															#line 607 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			distance = alternatives->getDistance(useAlt);
															#line 608 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!IFailureMonitor::failureMonitor().getState(stream->getEndpoint()).failed && (!firstRequestEndpoint.present() || stream->getEndpoint().token.first() != firstRequestEndpoint.get()))
															#line 1445 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
				break;
			}
															#line 611 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			nextAlt = (nextAlt + 1) % alternatives->size();
															#line 612 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 1453 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 613 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				triedAllOptions = TriedAllOptions::True;
															#line 1457 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 614 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			stream = nullptr;
															#line 615 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			distance = LBDistance::DISTANT;
															#line 1463 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 618 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (!stream && !firstRequestData.isValid())
															#line 1467 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 621 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			std::vector<Future<Void>> ok(alternatives->size());
															#line 622 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			for(int i = 0;i < ok.size();i++) {
															#line 623 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				ok[i] = IFailureMonitor::failureMonitor().onStateEqual(alternatives->get(i, channel).getEndpoint(), FailureStatus(false));
															#line 1475 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 627 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			Future<Void> okFuture = quorum(ok, 1);
															#line 629 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!alternatives->alwaysFresh())
															#line 1481 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 631 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (now() - g_network->networkInfo.newestAlternativesFailure > 1 || deterministicRandom()->random01() < 0.01)
															#line 1485 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 633 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					TraceEvent("AllAlternativesFailed").detail("Alternatives", alternatives->description());
															#line 1489 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
															#line 635 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_0 = allAlternativesFailedDelay(okFuture);
															#line 635 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1495 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 1;
															#line 635 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 0, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1500 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 637 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_1 = okFuture;
															#line 637 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1509 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 2;
															#line 637 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 1, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1514 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
		}
		else
		{
															#line 641 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!stream)
															#line 1522 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 643 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_2 = firstRequestData.response;
															#line 643 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 1528 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when3(__when_expr_2.get(), loopDepth); };
				static_cast<LoadBalanceActor*>(this)->actor_wait_state = 3;
															#line 643 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1533 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
															#line 649 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (firstRequestData.isValid())
															#line 1540 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 651 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (distance == LBDistance::DISTANT)
															#line 1544 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 652 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						TraceEvent("LBDistant2nd") .suppressFor(0.1) .detail("Distance", (int)distance) .detail("BackOff", backoff) .detail("TriedAllOptions", triedAllOptions) .detail("Alternatives", alternatives->description()) .detail("Token", stream->getEndpoint().token) .detail("Total", alternatives->size()) .detail("Best", alternatives->countBest()) .detail("Attempts", numAttempts);
															#line 1548 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
															#line 663 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					secondRequestData.startRequest(backoff, triedAllOptions, stream, request, model, alternatives, channel);
															#line 665 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					;
															#line 1554 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					loopDepth = a_body1loopBody1loopHead1(loopDepth);
				}
				else
				{
															#line 690 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					if (distance == LBDistance::DISTANT)
															#line 1561 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					{
															#line 691 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
						TraceEvent("LBDistant") .suppressFor(0.1) .detail("Distance", (int)distance) .detail("BackOff", backoff) .detail("TriedAllOptions", triedAllOptions) .detail("Alternatives", alternatives->description()) .detail("Token", stream->getEndpoint().token) .detail("Total", alternatives->size()) .detail("Best", alternatives->countBest()) .detail("Attempts", numAttempts);
															#line 1565 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					}
															#line 702 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					firstRequestData.startRequest(backoff, triedAllOptions, stream, request, model, alternatives, channel);
															#line 703 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					firstRequestEndpoint = stream->getEndpoint().token.first();
															#line 705 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					;
															#line 1573 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
					loopDepth = a_body1loopBody1loopHead2(loopDepth);
				}
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 741 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		nextAlt = (nextAlt + 1) % alternatives->size();
															#line 742 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (nextAlt == startAlt)
															#line 1587 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 743 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			triedAllOptions = TriedAllOptions::True;
															#line 1591 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 744 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		resetReply(request, taskID);
															#line 745 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 1597 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont11(int loopDepth) 
	{
															#line 640 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 1606 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont13(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont13(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont13(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont13(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont15(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont15(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont15(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont15(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont16(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 644 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1771 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 645 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1775 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 648 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1783 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont17(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 644 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1792 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 645 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1796 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 648 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1804 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when3(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont17(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1when3(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	int a_body1loopBody1cont18(int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont16(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont19(int loopDepth) 
	{
															#line 683 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 1882 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 684 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 1886 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont18(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopHead1(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1(int loopDepth) 
	{
															#line 666 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_3 = firstRequestData.response.isValid() ? firstRequestData.response : Never();
															#line 665 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 1905 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_3.isReady()) { if (__when_expr_3.isError()) return a_body1Catch1(__when_expr_3.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when1(__when_expr_3.get(), loopDepth); };
															#line 674 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_4 = secondRequestData.response;
															#line 1909 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_4.isReady()) { if (__when_expr_4.isError()) return a_body1Catch1(__when_expr_4.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody1when2(__when_expr_4.get(), loopDepth); };
		static_cast<LoadBalanceActor*>(this)->actor_wait_state = 4;
															#line 666 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_3.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 674 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_4.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 1916 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break1(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont19(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody1cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 668 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1944 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 669 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1948 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 672 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1956 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 668 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 1965 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 669 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1969 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 672 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 1977 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 675 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (secondRequestData.checkAndProcessResult(atMostOnce))
															#line 1986 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 676 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 1990 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody1when2(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 675 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (secondRequestData.checkAndProcessResult(atMostOnce))
															#line 2004 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 676 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2008 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
		return a_body1loopBody1break1(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	void a_exitChoose4() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >::remove();
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 3, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 3);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 3);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1loopBody1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 4, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 4);
		a_exitChoose4();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 4);

	}
	int a_body1loopBody1cont21(int loopDepth) 
	{
															#line 734 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2119 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 735 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2123 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont18(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopHead2(int loopDepth) 
	{
		int oldLoopDepth = ++loopDepth;
		while (loopDepth == oldLoopDepth) loopDepth = a_body1loopBody1loopBody2(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2(int loopDepth) 
	{
															#line 707 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_5 = firstRequestData.response;
															#line 706 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 2));
															#line 2142 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_5.isReady()) { if (__when_expr_5.isError()) return a_body1Catch1(__when_expr_5.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody2when1(__when_expr_5.get(), loopDepth); };
															#line 723 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<Void> __when_expr_6 = secondDelay;
															#line 2146 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_6.isReady()) { if (__when_expr_6.isError()) return a_body1Catch1(__when_expr_6.getError(), std::max(0, loopDepth - 2)); else return a_body1loopBody1loopBody2when2(__when_expr_6.get(), loopDepth); };
		static_cast<LoadBalanceActor*>(this)->actor_wait_state = 5;
															#line 707 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_5.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<LoadBalanceActor*>(this)));
															#line 723 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_6.addCallbackAndClear(static_cast<ActorCallback< LoadBalanceActor, 6, Void >*>(static_cast<LoadBalanceActor*>(this)));
															#line 2153 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1break2(int loopDepth) 
	{
		try {
			return a_body1loopBody1cont21(loopDepth);
		}
		catch (Error& error) {
			loopDepth = a_body1Catch1(error, std::max(0, loopDepth - 1));
		} catch (...) {
			loopDepth = a_body1Catch1(unknown_error(), std::max(0, loopDepth - 1));
		}

		return loopDepth;
	}
	int a_body1loopBody1loopBody2cont1(int loopDepth) 
	{
		if (loopDepth == 0) return a_body1loopBody1loopHead2(0);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 708 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (model)
															#line 2181 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 709 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier = std::max(model->secondMultiplier - FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_DECAY, 1.0);
															#line 711 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondBudget = std::min(model->secondBudget + FLOW_KNOBS->SECOND_REQUEST_BUDGET_GROWTH, FLOW_KNOBS->SECOND_REQUEST_MAX_BUDGET);
															#line 2187 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 716 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 2191 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 717 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2195 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 720 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 2203 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 708 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (model)
															#line 2212 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 709 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier = std::max(model->secondMultiplier - FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_DECAY, 1.0);
															#line 711 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondBudget = std::min(model->secondBudget + FLOW_KNOBS->SECOND_REQUEST_BUDGET_GROWTH, FLOW_KNOBS->SECOND_REQUEST_MAX_BUDGET);
															#line 2218 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 716 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (firstRequestData.checkAndProcessResult(atMostOnce))
															#line 2222 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 717 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<LoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~LoadBalanceActorState(); static_cast<LoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2226 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<LoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~LoadBalanceActorState();
			static_cast<LoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 720 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		firstRequestEndpoint = Optional<uint64_t>();
															#line 2234 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when2(Void const& _,int loopDepth) 
	{
															#line 724 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 725 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (model && model->secondBudget >= 1.0)
															#line 2245 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 726 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier += FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_GROWTH;
															#line 727 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondBudget -= 1.0;
															#line 2251 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1loopBody2when2(Void && _,int loopDepth) 
	{
															#line 724 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		secondDelay = Never();
															#line 725 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (model && model->secondBudget >= 1.0)
															#line 2264 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 726 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondMultiplier += FLOW_KNOBS->SECOND_REQUEST_MULTIPLIER_GROWTH;
															#line 727 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			model->secondBudget -= 1.0;
															#line 2270 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			return a_body1loopBody1break2(loopDepth==0?0:loopDepth-1); // break
		}
		loopDepth = a_body1loopBody1loopBody2cont1(loopDepth);

		return loopDepth;
	}
	void a_exitChoose5() 
	{
		if (static_cast<LoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<LoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >::remove();
		static_cast<LoadBalanceActor*>(this)->ActorCallback< LoadBalanceActor, 6, Void >::remove();

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 5, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 5);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 5);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 6, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_fire(ActorCallback< LoadBalanceActor, 6, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1loopBody1loopBody2when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
	void a_callback_error(ActorCallback< LoadBalanceActor, 6, Void >*,Error err) 
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), 6);
		a_exitChoose5();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), 6);

	}
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Reference<MultiInterface<Multi>> alternatives;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request> Interface::* channel;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Request request;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TaskPriority taskID;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	AtMostOnce atMostOnce;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	QueueModel* model;
															#line 453 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestData<Request, Interface, Multi> firstRequestData;
															#line 454 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestData<Request, Interface, Multi> secondRequestData;
															#line 456 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Optional<uint64_t> firstRequestEndpoint;
															#line 457 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Future<Void> secondDelay;
															#line 459 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Promise<Void> requestFinished;
															#line 460 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double startTime;
															#line 462 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TriedAllOptions triedAllOptions;
															#line 470 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int bestAlt;
															#line 471 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int nextAlt;
															#line 568 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int startAlt;
															#line 569 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int startDistance;
															#line 571 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int numAttempts;
															#line 572 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double backoff;
															#line 597 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request> const* stream;
															#line 598 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	LBDistance::Type distance;
															#line 2416 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via loadBalance()
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi>
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class LoadBalanceActor final : public Actor<REPLY_TYPE(Request)>, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 0, Void >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 1, Void >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 3, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 4, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 5, ErrorOr<REPLY_TYPE(Request)> >, public ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 6, Void >, public FastAllocated<LoadBalanceActor<Interface, Request, Multi>>, public LoadBalanceActorState<Interface, Request, Multi, LoadBalanceActor<Interface, Request, Multi>> {
															#line 2423 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<LoadBalanceActor<Interface, Request, Multi>>::operator new;
	using FastAllocated<LoadBalanceActor<Interface, Request, Multi>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Request)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 0, Void >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 1, Void >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 3, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 4, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 5, ErrorOr<REPLY_TYPE(Request)> >;
friend struct ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 6, Void >;
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	LoadBalanceActor(Reference<MultiInterface<Multi>> const& alternatives,RequestStream<Request> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False,QueueModel* const& model = nullptr) 
															#line 2440 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<REPLY_TYPE(Request)>(),
		   LoadBalanceActorState<Interface, Request, Multi, LoadBalanceActor<Interface, Request, Multi>>(alternatives, channel, request, taskID, atMostOnce, model)
	{
		fdb_probe_actor_enter("loadBalance", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("loadBalance");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("loadBalance", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		case 4: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 3, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		case 5: this->a_callback_error((ActorCallback< LoadBalanceActor<Interface, Request, Multi>, 5, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi>
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Request)> loadBalance( Reference<MultiInterface<Multi>> const& alternatives, RequestStream<Request> Interface::* const& channel, Request const& request = Request(), TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint, AtMostOnce const& atMostOnce = AtMostOnce::False, QueueModel* const& model = nullptr ) {
															#line 443 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	return Future<REPLY_TYPE(Request)>(new LoadBalanceActor<Interface, Request, Multi>(alternatives, channel, request, taskID, atMostOnce, model));
															#line 2474 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
}

#line 748 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"

// Subclasses must initialize all members in their default constructors
// Subclasses must serialize all members
struct BasicLoadBalancedReply {
	int processBusyTime;
	BasicLoadBalancedReply() : processBusyTime(0) {}
};

Optional<BasicLoadBalancedReply> getBasicLoadBalancedReply(const BasicLoadBalancedReply* reply);
Optional<BasicLoadBalancedReply> getBasicLoadBalancedReply(const void*);

// A simpler version of LoadBalance that does not send second requests where the list of servers are always fresh
															#line 2490 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
namespace {
// This generated class is to be used only via basicLoadBalance()
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi, class BasicLoadBalanceActor>
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class BasicLoadBalanceActorState {
															#line 2497 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	BasicLoadBalanceActorState(Reference<ModelInterface<Multi>> const& alternatives,RequestStream<Request> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False) 
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		 : alternatives(alternatives),
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   channel(channel),
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   request(request),
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   taskID(taskID),
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		   atMostOnce(atMostOnce)
															#line 2512 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
	{
		fdb_probe_actor_create("basicLoadBalance", reinterpret_cast<unsigned long>(this));

	}
	~BasicLoadBalanceActorState() 
	{
		fdb_probe_actor_destroy("basicLoadBalance", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 766 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			setReplyPriority(request, taskID);
															#line 767 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!alternatives)
															#line 2529 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 768 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				this->~BasicLoadBalanceActorState();
															#line 2533 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				static_cast<BasicLoadBalanceActor*>(this)->sendAndDelPromiseRef(Never());
				return 0;
			}
															#line 770 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			ASSERT(alternatives->size() && alternatives->alwaysFresh());
															#line 772 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			bestAlt = alternatives->getBest();
															#line 773 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			nextAlt = deterministicRandom()->randomInt(0, std::max(alternatives->size() - 1, 1));
															#line 774 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (nextAlt >= bestAlt)
															#line 2545 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 775 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				nextAlt++;
															#line 2549 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 777 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			startAlt = nextAlt;
															#line 778 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			startDistance = (bestAlt + alternatives->size() - startAlt) % alternatives->size();
															#line 780 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			numAttempts = 0;
															#line 781 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = 0;
															#line 782 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			useAlt = int();
															#line 783 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			;
															#line 2563 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
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
		this->~BasicLoadBalanceActorState();
		static_cast<BasicLoadBalanceActor*>(this)->sendErrorAndDelPromiseRef(error);
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
															#line 785 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		stream = nullptr;
															#line 786 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		for(int alternativeNum = 0;alternativeNum < alternatives->size();alternativeNum++) {
															#line 787 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			useAlt = nextAlt;
															#line 788 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (nextAlt == startAlt)
															#line 2599 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 789 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				useAlt = bestAlt;
															#line 2603 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
			else
			{
															#line 790 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if ((nextAlt + alternatives->size() - startAlt) % alternatives->size() <= startDistance)
															#line 2609 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				{
															#line 791 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
					useAlt = (nextAlt + alternatives->size() - 1) % alternatives->size();
															#line 2613 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				}
			}
															#line 793 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			stream = &alternatives->get(useAlt, channel);
															#line 794 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!IFailureMonitor::failureMonitor().getState(stream->getEndpoint()).failed)
															#line 2620 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
				break;
			}
															#line 796 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			nextAlt = (nextAlt + 1) % alternatives->size();
															#line 797 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			stream = nullptr;
															#line 2628 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 800 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (!stream)
															#line 2632 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 803 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			std::vector<Future<Void>> ok(alternatives->size());
															#line 804 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			for(int i = 0;i < ok.size();i++) {
															#line 805 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				ok[i] = IFailureMonitor::failureMonitor().onStateEqual(alternatives->get(i, channel).getEndpoint(), FailureStatus(false));
															#line 2640 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 808 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			StrictFuture<Void> __when_expr_0 = quorum(ok, 1);
															#line 808 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2646 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when1(__when_expr_0.get(), loopDepth); };
			static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 1;
															#line 808 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 0, Void >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2651 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			loopDepth = 0;
		}
		else
		{
															#line 812 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (backoff > 0.0)
															#line 2658 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 813 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				StrictFuture<Void> __when_expr_1 = delay(backoff);
															#line 813 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2664 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1when2(__when_expr_1.get(), loopDepth); };
				static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 2;
															#line 813 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 1, Void >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2669 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
				loopDepth = 0;
			}
			else
			{
				loopDepth = a_body1loopBody1cont10(loopDepth);
			}
		}

		return loopDepth;
	}
	int a_body1loopBody1cont1(int loopDepth) 
	{
															#line 843 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		nextAlt = (nextAlt + 1) % alternatives->size();
															#line 844 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		resetReply(request, taskID);
															#line 2686 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (loopDepth == 0) return a_body1loopHead1(0);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void const& _,int loopDepth) 
	{
															#line 810 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 2695 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont7(Void && _,int loopDepth) 
	{
															#line 810 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		numAttempts = 0;
															#line 2704 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont7(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 0, Void >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1loopBody1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 0);

	}
	int a_body1loopBody1cont10(int loopDepth) 
	{
															#line 816 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		StrictFuture<ErrorOr<REPLY_TYPE(Request)>> __when_expr_2 = stream->tryGetReply(request);
															#line 816 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), std::max(0, loopDepth - 1));
															#line 2778 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		if (__when_expr_2.isReady()) { if (__when_expr_2.isError()) return a_body1Catch1(__when_expr_2.getError(), std::max(0, loopDepth - 1)); else return a_body1loopBody1cont10when1(__when_expr_2.get(), loopDepth); };
		static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 3;
															#line 816 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		__when_expr_2.addCallbackAndClear(static_cast<ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*>(static_cast<BasicLoadBalanceActor*>(this)));
															#line 2783 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1loopBody1cont11(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont11(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(_, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1when2(Void && _,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont11(std::move(_), loopDepth);

		return loopDepth;
	}
	void a_exitChoose2() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1loopBody1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose2();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 1);

	}
	int a_body1loopBody1cont10cont1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
															#line 818 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (result.present())
															#line 2867 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 819 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			Optional<BasicLoadBalancedReply> loadBalancedReply = getBasicLoadBalancedReply(&result.get());
															#line 820 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (loadBalancedReply.present())
															#line 2873 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 821 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				alternatives->updateRecent(useAlt, loadBalancedReply.get().processBusyTime);
															#line 2877 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 824 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<BasicLoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~BasicLoadBalanceActorState(); static_cast<BasicLoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2881 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<BasicLoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~BasicLoadBalanceActorState();
			static_cast<BasicLoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 827 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (result.getError().code() != error_code_broken_promise && result.getError().code() != error_code_request_maybe_delivered)
															#line 2889 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 829 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(result.getError(), std::max(0, loopDepth - 1));
															#line 2893 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 832 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (atMostOnce)
															#line 2897 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 833 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(request_maybe_delivered(), std::max(0, loopDepth - 1));
															#line 2901 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 836 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2905 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 837 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2909 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10cont1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
															#line 818 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (result.present())
															#line 2919 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 819 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			Optional<BasicLoadBalancedReply> loadBalancedReply = getBasicLoadBalancedReply(&result.get());
															#line 820 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (loadBalancedReply.present())
															#line 2925 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			{
															#line 821 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
				alternatives->updateRecent(useAlt, loadBalancedReply.get().processBusyTime);
															#line 2929 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			}
															#line 824 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			if (!static_cast<BasicLoadBalanceActor*>(this)->SAV<REPLY_TYPE(Request)>::futures) { (void)(result.get()); this->~BasicLoadBalanceActorState(); static_cast<BasicLoadBalanceActor*>(this)->destroy(); return 0; }
															#line 2933 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
			new (&static_cast<BasicLoadBalanceActor*>(this)->SAV< REPLY_TYPE(Request) >::value()) REPLY_TYPE(Request)(result.get());
			this->~BasicLoadBalanceActorState();
			static_cast<BasicLoadBalanceActor*>(this)->finishSendAndDelPromiseRef();
			return 0;
		}
															#line 827 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (result.getError().code() != error_code_broken_promise && result.getError().code() != error_code_request_maybe_delivered)
															#line 2941 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 829 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(result.getError(), std::max(0, loopDepth - 1));
															#line 2945 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 832 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (atMostOnce)
															#line 2949 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 833 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			return a_body1Catch1(request_maybe_delivered(), std::max(0, loopDepth - 1));
															#line 2953 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
															#line 836 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
		if (++numAttempts >= alternatives->size())
															#line 2957 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		{
															#line 837 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
			backoff = std::min( FLOW_KNOBS->LOAD_BALANCE_MAX_BACKOFF, std::max(FLOW_KNOBS->LOAD_BALANCE_START_BACKOFF, backoff * FLOW_KNOBS->LOAD_BALANCE_BACKOFF_RATE));
															#line 2961 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		}
		loopDepth = a_body1loopBody1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10when1(ErrorOr<REPLY_TYPE(Request)> const& result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10cont1(result, loopDepth);

		return loopDepth;
	}
	int a_body1loopBody1cont10when1(ErrorOr<REPLY_TYPE(Request)> && result,int loopDepth) 
	{
		loopDepth = a_body1loopBody1cont10cont1(std::move(result), loopDepth);

		return loopDepth;
	}
	void a_exitChoose3() 
	{
		if (static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state > 0) static_cast<BasicLoadBalanceActor*>(this)->actor_wait_state = 0;
		static_cast<BasicLoadBalanceActor*>(this)->ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >::remove();

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> const& value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont10when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_fire(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,ErrorOr<REPLY_TYPE(Request)> && value) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1loopBody1cont10when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
	void a_callback_error(ActorCallback< BasicLoadBalanceActor, 2, ErrorOr<REPLY_TYPE(Request)> >*,Error err) 
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);
		a_exitChoose3();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), 2);

	}
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Reference<ModelInterface<Multi>> alternatives;
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request> Interface::* channel;
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	Request request;
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	TaskPriority taskID;
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	AtMostOnce atMostOnce;
															#line 772 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int bestAlt;
															#line 773 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int nextAlt;
															#line 777 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int startAlt;
															#line 778 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int startDistance;
															#line 780 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int numAttempts;
															#line 781 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	double backoff;
															#line 782 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	int useAlt;
															#line 785 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	RequestStream<Request> const* stream;
															#line 3056 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
};
// This generated class is to be used only via basicLoadBalance()
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi>
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
class BasicLoadBalanceActor final : public Actor<REPLY_TYPE(Request)>, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 0, Void >, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 1, Void >, public ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >, public FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi>>, public BasicLoadBalanceActorState<Interface, Request, Multi, BasicLoadBalanceActor<Interface, Request, Multi>> {
															#line 3063 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
public:
	using FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi>>::operator new;
	using FastAllocated<BasicLoadBalanceActor<Interface, Request, Multi>>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<REPLY_TYPE(Request)>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 0, Void >;
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 1, Void >;
friend struct ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >;
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	BasicLoadBalanceActor(Reference<ModelInterface<Multi>> const& alternatives,RequestStream<Request> Interface::* const& channel,Request const& request = Request(),TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint,AtMostOnce const& atMostOnce = AtMostOnce::False) 
															#line 3076 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
		 : Actor<REPLY_TYPE(Request)>(),
		   BasicLoadBalanceActorState<Interface, Request, Multi, BasicLoadBalanceActor<Interface, Request, Multi>>(alternatives, channel, request, taskID, atMostOnce)
	{
		fdb_probe_actor_enter("basicLoadBalance", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("basicLoadBalance");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("basicLoadBalance", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 0, Void >*)0, actor_cancelled()); break;
		case 2: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 1, Void >*)0, actor_cancelled()); break;
		case 3: this->a_callback_error((ActorCallback< BasicLoadBalanceActor<Interface, Request, Multi>, 2, ErrorOr<REPLY_TYPE(Request)> >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
template <class Interface, class Request, class Multi>
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
[[nodiscard]] Future<REPLY_TYPE(Request)> basicLoadBalance( Reference<ModelInterface<Multi>> const& alternatives, RequestStream<Request> Interface::* const& channel, Request const& request = Request(), TaskPriority const& taskID = TaskPriority::DefaultPromiseEndpoint, AtMostOnce const& atMostOnce = AtMostOnce::False ) {
															#line 760 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"
	return Future<REPLY_TYPE(Request)>(new BasicLoadBalanceActor<Interface, Request, Multi>(alternatives, channel, request, taskID, atMostOnce));
															#line 3108 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.h"
}

#line 847 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.h"

#include "flow/unactorcompiler.h"

#endif
