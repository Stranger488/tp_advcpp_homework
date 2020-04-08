#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include <functional>
#include "Connection.hpp"
#include "FileDescriptor.hpp"

namespace Tcp_epoll {

using Callback = std::function<void(Connection&)>;

static const size_t DEFAULT_MAX_CONNECTION_NUMBER = 2;

class Server {
public:
    Server(const std::string& address, uint16_t port, Callback handler);
    ~Server() noexcept;

    void open(const std::string& address, uint16_t port);
    Connection accept();
    void set_max_connect(size_t max_connect);

    bool is_opened() const;
    void close();

    void create_epoll();
    void add_epoll(int fd, uint32_t events);
    void modify_epoll(int fd, uint32_t events);

    void handle_client(int fd, int event);
    void accept_clients();

private:
    FileDescriptor fd_;
    FileDescriptor epoll_fd_;

    size_t max_connect_ = DEFAULT_MAX_CONNECTION_NUMBER;

    bool is_opened_;
};

} // namespace Tcp_epoll


#endif // SERVER_HPP
