#include <stdexcept>
#include <cstring>
#include <unistd.h>

#include <arpa/inet.h>

#include "Server.hpp"

namespace Tcp {

Server::Server(const std::string& address, uint16_t port) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

    open(address, port);
    is_opened_ = true;
}

Server::~Server() noexcept {
    close();
}

void Server::open(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int res = inet_aton(address.c_str(), &addr.sin_addr);
    if (res == 0) { // invalid IP address is given
        close();
        throw std::runtime_error(std::string("Error while converting IP address: invalid IP address."));
    } else if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    res = bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }

    set_max_connect(max_connect_);
}

void Server::set_max_connect(size_t max_connect) {
    int res = listen(fd_, max_connect);
    if (res < 0) {
        close();
        throw std::runtime_error(std::strerror(errno));
    }
}

Connection Server::accept() {
    sockaddr_in client_addr{};
    socklen_t addr_size = sizeof(client_addr);

    int sock_fd = ::accept(fd_,
                     reinterpret_cast<sockaddr*>(&client_addr),
                     &addr_size);
    if (sock_fd < 0) {
        throw std::runtime_error(std::strerror(errno));
    }


    std::string client_addr_string(addr_size, '\0');
    const char* res_addr = inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_string.data(), addr_size);
    if (res_addr == nullptr) {
        ::close(sock_fd);
        throw std::runtime_error(std::strerror(errno));
    }

    uint16_t client_addr_port = client_addr.sin_port;

    return Connection(client_addr_string, client_addr_port, sock_fd);
}

bool Server::is_opened() const {
    return is_opened_;
}

void Server::close() {
    ::close(fd_);

    is_opened_ = false;
}

} // namespace Tcp
