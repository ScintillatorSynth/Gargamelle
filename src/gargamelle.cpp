#include "DumpHandler.hpp"
#include "HandlerFactory.hpp"
#include "HelloHandler.hpp"

#include <folly/portability/GFlags.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <wangle/ssl/SSLContextConfig.h>

DEFINE_int32(port, 8080, "Which port to bind for incoming HTTP(S) traffic.");
DEFINE_string(dumpPath, "", "Root path for saving incoming crash dumps.");
DEFINE_int32(numThreads, 2, "Number of threads to run on the server.");

DEFINE_bool(useSSL, false, "Enable SSL on the bound connection.");
DEFINE_string(certificatePath, "", "Path to the SSL certificate.");
DEFINE_string(keyPath, "", "Path to the SSL key.");
DEFINE_string(keyPassword, "", "SSL key password.");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    auto diskIOThreadPool = std::make_shared<folly::CPUThreadPoolExecutor>(
            FLAGS_numThreads, std::make_shared<folly::NamedThreadFactory>("StaticDiskIOThread"));
    folly::setCPUExecutor(diskIOThreadPool);


    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        { folly::SocketAddress("0.0.0.0", FLAGS_port, true), proxygen::HTTPServer::Protocol::HTTP }
    };

    if (FLAGS_useSSL) {
        wangle::SSLContextConfig sslConfig;
        sslConfig.setCertificate(FLAGS_certificatePath, FLAGS_keyPath, FLAGS_keyPassword);
        IPs[0].sslConfigs.push_back(sslConfig);
    }

    proxygen::HTTPServerOptions options;
    options.threads = FLAGS_numThreads;
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = true;
    options.handlerFactories = proxygen::RequestHandlerChain()
        .addThen<ggml::handler::HandlerFactory>(FLAGS_dumpPath).build();
    options.h2cEnabled = false;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    std::thread t([&] () {
            LOG(INFO) << "starting server.";
        server.start();
    });

    t.join();
    return 0;
}
