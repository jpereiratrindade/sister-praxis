// SPDX-License-Identifier: GPL-3.0-or-later
#include "sister/praxis/http.hpp"
#include "sister/praxis/runtime.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

namespace {

volatile std::sig_atomic_t g_stop = 0;

void handle_signal(int) {
    g_stop = 1;
}

sister::praxis::runtime::ServiceRegistry make_registry() {
    using namespace sister::praxis::runtime;
    ServiceRegistry registry;
    registry.register_service(
        ServiceDescriptor{
            .id = "demo.state.derive",
            .owner = "demo-domain",
            .kind = TransformationKind::derivation,
            .contract_ref = "demo.state.derive@v1",
            .implementation_ref = "builtin:demo-state-derive@1",
            .version = "1.0.0",
            .description = "demonstrates domain-owned semantics executed by the shared Praxis runtime",
        },
        [](const TransformationRequest& request) {
            return TransformationOutput{
                .successor_state_id = request.predecessor_state_id + ".successor",
                .output_refs = {"artifact://demo/derived-state"},
                .provenance_ref = "prov://demo/" + request.run_id,
            };
        });
    return registry;
}

struct Options {
    std::string bind_address{"127.0.0.1"};
    unsigned short port{8093};
    std::string web_index{"web/index.html"};
};

Options parse_options(const int argc, char** argv) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg{argv[i]};
        if (arg == "--bind" && i + 1 < argc) {
            options.bind_address = argv[++i];
        } else if (arg == "--port" && i + 1 < argc) {
            const auto value = std::stoul(argv[++i]);
            if (value == 0U || value > 65535U) {
                throw std::invalid_argument("port out of range");
            }
            options.port = static_cast<unsigned short>(value);
        } else if (arg == "--web-index" && i + 1 < argc) {
            options.web_index = argv[++i];
        } else {
            throw std::invalid_argument(
                "usage: sister-praxis-http [--bind 127.0.0.1] [--port 8093] [--web-index path]");
        }
    }
    if (options.bind_address != "127.0.0.1") {
        throw std::invalid_argument("A0-R2 HTTP boundary must bind to 127.0.0.1");
    }
    return options;
}


std::string read_text_file(const std::string& path) {
    std::ifstream input{path, std::ios::binary};
    if (!input) {
        throw std::runtime_error("cannot open web index: " + path);
    }
    return std::string{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

std::pair<std::string, std::string> parse_request_line(const std::string_view request) {
    const auto line_end = request.find("\r\n");
    const auto line = request.substr(0, line_end);
    const auto first_space = line.find(' ');
    const auto second_space = first_space == std::string_view::npos
        ? std::string_view::npos
        : line.find(' ', first_space + 1U);
    if (first_space == std::string_view::npos || second_space == std::string_view::npos) {
        throw std::invalid_argument("malformed request line");
    }
    return {
        std::string{line.substr(0, first_space)},
        std::string{line.substr(first_space + 1U, second_space - first_space - 1U)},
    };
}

void write_all(const int fd, const std::string& payload) {
    std::size_t written = 0;
    while (written < payload.size()) {
        const auto rc = ::send(fd, payload.data() + written, payload.size() - written, MSG_NOSIGNAL);
        if (rc < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::runtime_error(std::string{"send failed: "} + std::strerror(errno));
        }
        written += static_cast<std::size_t>(rc);
    }
}

} // namespace

int main(int argc, char** argv) {
    try {
        const auto options = parse_options(argc, argv);
        const auto registry = make_registry();
        const auto index_html = read_text_file(options.web_index);
        const sister::praxis::http::Application app{registry, index_html};

        std::signal(SIGINT, handle_signal);
        std::signal(SIGTERM, handle_signal);

        const int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error(std::string{"socket failed: "} + std::strerror(errno));
        }

        const int yes = 1;
        if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
            ::close(server_fd);
            throw std::runtime_error(std::string{"setsockopt failed: "} + std::strerror(errno));
        }

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(options.port);
        if (::inet_pton(AF_INET, options.bind_address.c_str(), &address.sin_addr) != 1) {
            ::close(server_fd);
            throw std::invalid_argument("invalid bind address");
        }

        if (::bind(server_fd, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) < 0) {
            const auto message = std::string{"bind failed: "} + std::strerror(errno);
            ::close(server_fd);
            throw std::runtime_error(message);
        }
        if (::listen(server_fd, 32) < 0) {
            const auto message = std::string{"listen failed: "} + std::strerror(errno);
            ::close(server_fd);
            throw std::runtime_error(message);
        }

        std::cout << "SisTer-Praxis HTTP A0-R2 listening on "
                  << options.bind_address << ':' << options.port << '\n';
        std::cout.flush();

        while (g_stop == 0) {
            pollfd ready{.fd = server_fd, .events = POLLIN, .revents = 0};
            const int poll_rc = ::poll(&ready, 1, 250);
            if (poll_rc < 0) {
                if (errno == EINTR) {
                    continue;
                }
                const auto message = std::string{"poll failed: "} + std::strerror(errno);
                ::close(server_fd);
                throw std::runtime_error(message);
            }
            if (poll_rc == 0) {
                continue;
            }

            const int client_fd = ::accept(server_fd, nullptr, nullptr);
            if (client_fd < 0) {
                if (errno == EINTR) {
                    continue;
                }
                const auto message = std::string{"accept failed: "} + std::strerror(errno);
                ::close(server_fd);
                throw std::runtime_error(message);
            }

            try {
                std::string request;
                request.resize(16384U);
                const auto received = ::recv(client_fd, request.data(), request.size(), 0);
                if (received > 0) {
                    request.resize(static_cast<std::size_t>(received));
                    const auto [method, path] = parse_request_line(request);
                    write_all(client_fd, sister::praxis::http::serialize_response(app.handle(method, path)));
                }
            } catch (const std::exception&) {
                const sister::praxis::http::Response bad{
                    .status = 404,
                    .body = R"({"error":"bad_request"})",
                };
                try {
                    write_all(client_fd, sister::praxis::http::serialize_response(bad));
                } catch (const std::exception&) {
                }
            }
            ::close(client_fd);
        }

        ::close(server_fd);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "sister-praxis-http: " << error.what() << '\n';
        return 1;
    }
}
