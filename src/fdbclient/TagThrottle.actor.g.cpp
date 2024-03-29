#define POST_ACTOR_COMPILER 1
#line 1 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
/*
 * TagThrottle.actor.cpp
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

#include "fdbclient/TagThrottle.actor.h"
#include "fdbclient/CommitProxyInterface.h"
#include "fdbclient/DatabaseContext.h"

#include "flow/actorcompiler.h" // has to be last include

void TagSet::addTag(TransactionTagRef tag) {
	ASSERT(CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH < 256); // Tag length is encoded with a single byte
	ASSERT(CLIENT_KNOBS->MAX_TAGS_PER_TRANSACTION < 256); // Number of tags is encoded with a single byte

	if (tag.size() > CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH) {
		throw tag_too_long();
	}
	if (tags.size() >= CLIENT_KNOBS->MAX_TAGS_PER_TRANSACTION) {
		throw too_many_tags();
	}

	TransactionTagRef tagRef(arena, tag);
	auto it = find(tags.begin(), tags.end(), tagRef);
	if (it == tags.end()) {
		tags.push_back(std::move(tagRef));
		bytes += tag.size();
	}
}

size_t TagSet::size() const {
	return tags.size();
}

std::string TagSet::toString(Capitalize capitalize) const {
	ASSERT(!tags.empty());
	if (tags.size() == 1) {
		std::string start = capitalize ? "Tag" : "tag";
		return format("%s `%s'", start.c_str(), tags[0].toString().c_str());
	}
	std::string result = capitalize ? "Tags (" : "tags (";
	for (int index = 0; index < tags.size() - 1; ++index) {
		result += format("`%s', ", tags[index].toString().c_str());
	}
	return result + format("`%s')", tags.back().toString().c_str());
}

// Format for throttle key:
//
// tagThrottleKeysPrefix + [auto-throttled (1-byte 0/1)] + [priority (1-byte)] + [tag list]
// tag list consists of 1 or more consecutive tags, each encoded as:
// tag.size() (1 byte) + tag's bytes. For example, tag 'foo' is: \x03foo
// The tags are listed in sorted order
//
// Currently, the throttle API supports only 1 tag per throttle
Key TagThrottleKey::toKey() const {
	ASSERT(CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH < 256);
	ASSERT(tags.size() > 0);

	ASSERT(tags.size() == 1); // SOMEDAY: support multiple tags per throttle

	int size = tagThrottleKeysPrefix.size() + tags.size() + 2;
	for (auto tag : tags) {
		ASSERT(tag.size() <= CLIENT_KNOBS->MAX_TRANSACTION_TAG_LENGTH);
		size += tag.size();
	}

	Key result;

	uint8_t* str = new (result.arena()) uint8_t[size];
	result.contents() = StringRef(str, size);

	memcpy(str, tagThrottleKeysPrefix.begin(), tagThrottleKeysPrefix.size());
	str += tagThrottleKeysPrefix.size();

	*(str++) = (uint8_t)throttleType;
	*(str++) = (uint8_t)priority;

	for (auto tag : tags) {
		*(str++) = (uint8_t)tag.size();
		if (tag.size() > 0) {
			memcpy(str, tag.begin(), tag.size());
			str += tag.size();
		}
	}

	return result;
}

TagThrottleKey TagThrottleKey::fromKey(const KeyRef& key) {
	const uint8_t* str = key.substr(tagThrottleKeysPrefix.size()).begin();
	TagThrottleType throttleType = TagThrottleType(*(str++));
	TransactionPriority priority = TransactionPriority(*(str++));
	TagSet tags;

	while (str < key.end()) {
		uint8_t size = *(str++);
		tags.addTag(TransactionTagRef(str, size));
		str += size;
	}

	return TagThrottleKey(tags, throttleType, priority);
}

TagThrottleValue TagThrottleValue::fromValue(const ValueRef& value) {
	TagThrottleValue throttleValue;
	BinaryReader reader(value, IncludeVersion(ProtocolVersion::withTagThrottleValueReason()));
	reader >> throttleValue;
	return throttleValue;
}

FDB_DEFINE_BOOLEAN_PARAM(ContainsRecommended);
FDB_DEFINE_BOOLEAN_PARAM(Capitalize);

															#line 132 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
namespace {
// This generated class is to be used only via flowTestCase130()
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
template <class FlowTestCase130Actor>
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
class FlowTestCase130ActorState {
															#line 139 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
public:
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
	FlowTestCase130ActorState(UnitTestParameters const& params) 
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
		 : params(params)
															#line 146 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
	{
		fdb_probe_actor_create("flowTestCase130", reinterpret_cast<unsigned long>(this));

	}
	~FlowTestCase130ActorState() 
	{
		fdb_probe_actor_destroy("flowTestCase130", reinterpret_cast<unsigned long>(this));

	}
	int a_body1(int loopDepth=0) 
	{
		try {
			{
															#line 132 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				TagSet tagSet;
															#line 133 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("a"_sr);
															#line 134 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagSet.toString() == "tag `a'");
															#line 135 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagSet.toString(Capitalize::True) == "Tag `a'");
															#line 168 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
			}
			{
															#line 139 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				TagSet tagSet;
															#line 140 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("a"_sr);
															#line 141 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				tagSet.addTag("b"_sr);
															#line 142 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				auto tagString = tagSet.toString();
															#line 143 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				ASSERT(tagString == "tags (`a', `b')" || tagString == "tags (`b', `a')");
															#line 144 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				auto capitalizedTagString = tagSet.toString(Capitalize::True);
															#line 145 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
				ASSERT(capitalizedTagString == "Tags (`a', `b')" || capitalizedTagString == "Tags (`b', `a')");
															#line 185 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
			}
															#line 147 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
			if (!static_cast<FlowTestCase130Actor*>(this)->SAV<Void>::futures) { (void)(Void()); this->~FlowTestCase130ActorState(); static_cast<FlowTestCase130Actor*>(this)->destroy(); return 0; }
															#line 189 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
			new (&static_cast<FlowTestCase130Actor*>(this)->SAV< Void >::value()) Void(Void());
			this->~FlowTestCase130ActorState();
			static_cast<FlowTestCase130Actor*>(this)->finishSendAndDelPromiseRef();
			return 0;
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
		this->~FlowTestCase130ActorState();
		static_cast<FlowTestCase130Actor*>(this)->sendErrorAndDelPromiseRef(error);
		loopDepth = 0;

		return loopDepth;
	}
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
	UnitTestParameters params;
															#line 213 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
};
// This generated class is to be used only via flowTestCase130()
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
class FlowTestCase130Actor final : public Actor<Void>, public FastAllocated<FlowTestCase130Actor>, public FlowTestCase130ActorState<FlowTestCase130Actor> {
															#line 218 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
public:
	using FastAllocated<FlowTestCase130Actor>::operator new;
	using FastAllocated<FlowTestCase130Actor>::operator delete;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
	void destroy() override { ((Actor<Void>*)this)->~Actor(); operator delete(this); }
#pragma clang diagnostic pop
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
	FlowTestCase130Actor(UnitTestParameters const& params) 
															#line 228 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
		 : Actor<Void>(),
		   FlowTestCase130ActorState<FlowTestCase130Actor>(params)
	{
		fdb_probe_actor_enter("flowTestCase130", reinterpret_cast<unsigned long>(this), -1);
		#ifdef ENABLE_SAMPLING
		this->lineage.setActorName("flowTestCase130");
		LineageScope _(&this->lineage);
		#endif
		this->a_body1();
		fdb_probe_actor_exit("flowTestCase130", reinterpret_cast<unsigned long>(this), -1);

	}
	void cancel() override
	{
		auto wait_state = this->actor_wait_state;
		this->actor_wait_state = -1;
		switch (wait_state) {
		}

	}
};
}
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
static Future<Void> flowTestCase130( UnitTestParameters const& params ) {
															#line 130 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
	return Future<Void>(new FlowTestCase130Actor(params));
															#line 255 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.g.cpp"
}
ACTOR_TEST_CASE(flowTestCase130, "TagSet/toString")

#line 149 "/usr/src/libfdb_c/fdbclient/TagThrottle.actor.cpp"
