// Copyright (C) 2019-2025 Zilliz. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under the License

#include "cachinglayer/SharedLoad.h"

#include <folly/executors/thread_factory/InitThreadFactory.h>
#include <folly/executors/thread_factory/NamedThreadFactory.h>
#include <folly/futures/ThreadWheelTimekeeper.h>

namespace milvus::cachinglayer::internal {
folly::Timekeeper&
SharedLoadTimekeeper() {
    static folly::ThreadWheelTimekeeper timekeeper;
    return timekeeper;
}

folly::Executor&
SharedLoadExecutor() {
    static thread_local bool load_worker = false;
    // A synchronous translator may load another slot. Do not queue that nested
    // producer behind the workers blocked waiting for it. It still owns its token
    // and context; the outer producer retains its resources until the nested call drains.
    if (load_worker)
        return folly::InlineExecutor::instance();
    static folly::CPUThreadPoolExecutor executor(
        8, std::make_shared<folly::InitThreadFactory>(std::make_shared<folly::NamedThreadFactory>("mcl_load"),
                                                      [] { load_worker = true; }));
    return executor;
}

SharedLoad::SharedLoad(const OpContext* caller) : context(cancellation.getToken()) {
    if (caller) {
        context.build_id = caller->build_id;
        context.runtime_load_priority = caller->runtime_load_priority;
        context.coload_fields = caller->coload_fields;
        context.pinned_segment_state = caller->pinned_segment_state;
        context.pinned_state_owner = caller->pinned_state_owner;
        context.trace_context = caller->trace_context;
        context.trace_span = caller->trace_span;
    }
}
}  // namespace milvus::cachinglayer::internal
