#ifndef SRC_PATHS_HPP_
#define SRC_PATHS_HPP_

namespace ggml { namespace handler {

enum Path { kDump, kHello, kNotFound };

Path getPath(const std::string& path);

} // namespace handler
} // namespace ggml

#endif // SRC_PATHS_HPP_
