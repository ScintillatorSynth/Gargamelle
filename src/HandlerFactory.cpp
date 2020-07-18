#include "HandlerFactory.hpp"

#include "DumpHandler.hpp"
#include "HelloHandler.hpp"
#include "Paths.hpp"

namespace ggml { namespace handler {

HandlerFactory::HandlerFactory(const std::string& dumpPath): m_dumpPath(dumpPath) {}

void HandlerFactory::onServerStart(folly::EventBase*) noexcept {}

void HandlerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* HandlerFactory::onRequest(proxygen::RequestHandler*,
        proxygen::HTTPMessage* message) noexcept {
    return new DumpHandler(m_dumpPath);
}


} // namespace handler
} // namespace ggml
