#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
/*
 * LoadBalance.actor.cpp
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

#include "fdbrpc/LoadBalance.actor.h"
#include "flow/flow.h"
#include "flow/actorcompiler.h" // This must be the last #include.

FDB_DEFINE_BOOLEAN_PARAM(AtMostOnce);
FDB_DEFINE_BOOLEAN_PARAM(TriedAllOptions);

// Throwing all_alternatives_failed will cause the client to issue a GetKeyLocationRequest to the proxy, so this actor
// attempts to limit the number of these errors thrown by a single client to prevent it from saturating the proxies with
// these requests
															#line 33 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
namespace {
// This generated class is to be used only via allAlternativesFailedDelay()
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
template <class AllAlternativesFailedDelayActor>
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
class AllAlternativesFailedDelayActorState {
															#line 40 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
public:
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
	AllAlternativesFailedDelayActorState(Future<Void> const& okFuture) 
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
		 : okFuture(okFuture)
															#line 47 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
	{
		fdb_probe_actor_create("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this));

	}
	~AllAlternativesFailedDelayActorState() 
	{
		fdb_probe_actor_destroy("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
															#line 32 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			if (now() - g_network->networkInfo.newestAlternativesFailure > FLOW_KNOBS->ALTERNATIVES_FAILURE_RESET_TIME)
															#line 62 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			{
															#line 33 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
				g_network->networkInfo.oldestAlternativesFailure = now();
															#line 66 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			}
															#line 36 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			double delay = FLOW_KNOBS->ALTERNATIVES_FAILURE_MIN_DELAY;
															#line 37 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			if (now() - g_network->networkInfo.lastAlternativesFailureSkipDelay > FLOW_KNOBS->ALTERNATIVES_FAILURE_SKIP_DELAY)
															#line 72 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			{
															#line 38 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
				g_network->networkInfo.lastAlternativesFailureSkipDelay = now();
															#line 76 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			}
			else
			{
															#line 40 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
				double elapsed = now() - g_network->networkInfo.oldestAlternativesFailure;
															#line 41 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
				delay = std::max(delay, std::min(elapsed * FLOW_KNOBS->ALTERNATIVES_FAILURE_DELAY_RATIO, FLOW_KNOBS->ALTERNATIVES_FAILURE_MAX_DELAY));
															#line 44 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
				delay = std::max(delay, std::min(elapsed * FLOW_KNOBS->ALTERNATIVES_FAILURE_SLOW_DELAY_RATIO, FLOW_KNOBS->ALTERNATIVES_FAILURE_SLOW_MAX_DELAY));
															#line 86 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			}
															#line 49 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			g_network->networkInfo.newestAlternativesFailure = now();
															#line 52 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			StrictFuture<Void> __when_expr_0 = okFuture;
															#line 51 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			if (static_cast<AllAlternativesFailedDelayActor*>(this)->actor_wait_state < 0) return a_body1Catch1(actor_cancelled(), loopDepth);
															#line 94 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			if (__when_expr_0.isReady()) { if (__when_expr_0.isError()) return a_body1Catch1(__when_expr_0.getError(), loopDepth); else return a_body1when1(__when_expr_0.get(), loopDepth); };
															#line 53 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			StrictFuture<Void> __when_expr_1 = ::delayJittered(delay);
															#line 98 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
			if (__when_expr_1.isReady()) { if (__when_expr_1.isError()) return a_body1Catch1(__when_expr_1.getError(), loopDepth); else return a_body1when2(__when_expr_1.get(), loopDepth); };
			static_cast<AllAlternativesFailedDelayActor*>(this)->actor_wait_state = 1;
															#line 52 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			__when_expr_0.addCallbackAndClear(static_cast<ActorCallback< AllAlternativesFailedDelayActor, 0, Void >*>(static_cast<AllAlternativesFailedDelayActor*>(this)));
															#line 53 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
			__when_expr_1.addCallbackAndClear(static_cast<ActorCallback< AllAlternativesFailedDelayActor, 1, Void >*>(static_cast<AllAlternativesFailedDelayActor*>(this)));
															#line 105 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
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
		this->~AllAlternativesFailedDelayActorState();
		static_cast<AllAlternativesFailedDelayActor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
	int a_body1cont1(int loopDepth) 
	{
															#line 57 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
		if (!static_cast<AllAlternativesFailedDelayActor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~AllAlternativesFailedDelayActorState(); static_cast<AllAlternativesFailedDelayActor*>(this)->destroy(); return 0; }
															#line 128 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
		new (&static_cast<AllAlternativesFailedDelayActor*>(this)->SAV< Void >::value()) Void(Void());
		this->~AllAlternativesFailedDelayActorState();
		static_cast<AllAlternativesFailedDelayActor*>(this)->finishSendAndDelPromiseRef();
		return 0;

		return loopDepth;
	}
	int a_body1when1(Void const& _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when1(Void && _,int loopDepth) 
	{
		loopDepth = a_body1cont1(loopDepth);

		return loopDepth;
	}
	int a_body1when2(Void const& _,int loopDepth) 
	{
															#line 54 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
		return a_body1Catch1(all_alternatives_failed(), loopDepth);
															#line 152 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"

		return loopDepth;
	}
	int a_body1when2(Void && _,int loopDepth) 
	{
															#line 54 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
		return a_body1Catch1(all_alternatives_failed(), loopDepth);
															#line 160 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"

		return loopDepth;
	}
	void a_exitChoose1() 
	{
		if (static_cast<AllAlternativesFailedDelayActor*>(this)->actor_wait_state > 0) static_cast<AllAlternativesFailedDelayActor*>(this)->actor_wait_state = 0;
		static_cast<AllAlternativesFailedDelayActor*>(this)->ActorCallback< AllAlternativesFailedDelayActor, 0, Void >::remove();
		static_cast<AllAlternativesFailedDelayActor*>(this)->ActorCallback< AllAlternativesFailedDelayActor, 1, Void >::remove();

	}
	void a_callback_fire(ActorCallback< AllAlternativesFailedDelayActor, 0, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AllAlternativesFailedDelayActor, 0, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1when1(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_error(ActorCallback< AllAlternativesFailedDelayActor, 0, Void >*,Error err) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 0);

	}
	void a_callback_fire(ActorCallback< AllAlternativesFailedDelayActor, 1, Void >*,Void const& value) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(value, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_fire(ActorCallback< AllAlternativesFailedDelayActor, 1, Void >*,Void && value) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1when2(std::move(value), 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);

	}
	void a_callback_error(ActorCallback< AllAlternativesFailedDelayActor, 1, Void >*,Error err) 
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);
		a_exitChoose1();
		try {
			a_body1Catch1(err, 0);
		}
		catch (Error& error) {
			a_body1Catch1(error, 0);
		} catch (...) {
			a_body1Catch1(unknown_error(), 0);
		}
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), 1);

	}
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
	Future<Void> okFuture;
															#line 263 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
};
// This generated class is to be used only via allAlternativesFailedDelay()
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
class AllAlternativesFailedDelayActor final : public Actor<Void>, public ActorCallback< AllAlternativesFailedDelayActor, 0, Void >, public ActorCallback< AllAlternativesFailedDelayActor, 1, Void >, public FastAllocated<AllAlternativesFailedDelayActor>, public AllAlternativesFailedDelayActorState<AllAlternativesFailedDelayActor> {
															#line 268 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
public:
	using FastAllocated<AllAlternativesFailedDelayActor>::operator new;
	using FastAllocated<AllAlternativesFailedDelayActor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
friend struct ActorCallback< AllAlternativesFailedDelayActor, 0, Void >;
friend struct ActorCallback< AllAlternativesFailedDelayActor, 1, Void >;
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
	AllAlternativesFailedDelayActor(Future<Void> const& okFuture) 
															#line 280 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
		 : Actor<Void>(),
		   AllAlternativesFailedDelayActorState<AllAlternativesFailedDelayActor>(okFuture)
	{
		fdb_probe_actor_enter("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("allAlternativesFailedDelay");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("allAlternativesFailedDelay", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		case 1: this->a_callback_error((ActorCallback< AllAlternativesFailedDelayActor, 0, Void >*)0, actor_cancelled()); break;
		}

	}
};
}
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
[[nodiscard]] Future<Void> allAlternativesFailedDelay( Future<Void> const& okFuture ) {
															#line 31 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
	return Future<Void>(new AllAlternativesFailedDelayActor(okFuture));
															#line 308 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.g.cpp"
}

#line 59 "/usr/src/libfdb_c/fdbrpc/LoadBalance.actor.cpp"
