#ifndef SRC_DUMP_HANDLER_HPP_
#define SRC_DUMP_HANDLER_HPP_

#include <proxygen/httpserver/RequestHandler.h>

namespace ggml { namespace handler {

class DumpHandler : public proxygen::RequestHandler {
public:
    explicit DumpHandler(const std::string& dumpPath);

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;

private:
    void writeFile(folly::EventBase* evb);
    bool m_valid;
    std::string m_dumpPath;

    std::string m_guid;
    int m_dumpFile;
    std::unique_ptr<folly::IOBuf> m_body;
};


} // namespace handler
} // namespace ggml

#endif // SRC_DUMP_HANDLER_HPP_
