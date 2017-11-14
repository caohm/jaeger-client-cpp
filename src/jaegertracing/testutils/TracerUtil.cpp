/*
 * Copyright (c) 2017 Uber Technologies, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "jaegertracing/testutils/TracerUtil.h"

namespace jaegertracing {
namespace testutils {
namespace TracerUtil {

std::shared_ptr<ResourceHandle> installGlobalTracer()
{
    std::unique_ptr<ResourceHandle> handle(new ResourceHandle());
    handle->_mockAgent->start();
    std::ostringstream samplingServerURLStream;
    samplingServerURLStream
        << "http://" << handle->_mockAgent->samplingServerAddress().authority();
    Config config(
        false,
        samplers::Config("const",
                         1,
                         samplingServerURLStream.str(),
                         0,
                         samplers::Config::Clock::duration()),
        reporters::Config(0,
                          reporters::Config::Clock::duration(),
                          false,
                          handle->_mockAgent->spanServerAddress().authority()),
        propagation::HeadersConfig(),
        baggage::RestrictionsConfig());

    auto tracer = Tracer::make("test-service", config, logging::nullLogger());
    opentracing::Tracer::InitGlobal(tracer);
    return std::move(handle);
}

}  // namespace TracerUtil
}  // namespace testutils
}  // namespace jaegertracing