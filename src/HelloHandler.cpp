#include "HelloHandler.hpp"

#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

namespace garg { namespace handler {

HelloHandler::HelloHandler() {}

void HelloHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept {
    LOG(INFO) << "got hello";
}

void HelloHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
}

void HelloHandler::onEOM() noexcept {
    proxygen::ResponseBuilder(downstream_).status(200, "OK").body("Hello!").sendWithEOM();
}

void HelloHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
}

void HelloHandler::requestComplete() noexcept {
    delete this;
}

void HelloHandler::onError(proxygen::ProxygenError err) noexcept {
    LOG(ERROR) << "hellohandler got error";
    delete this;
}

// ==== HelloHandlerFactory
HelloHandlerFactory::HelloHandlerFactory() {}
void HelloHandlerFactory::onServerStart(folly::EventBase*) noexcept {}
void HelloHandlerFactory::onServerStop() noexcept {}
proxygen::RequestHandler* HelloHandlerFactory::onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept {
    return new HelloHandler();
}

} // namespace handler
} // namespace garg
