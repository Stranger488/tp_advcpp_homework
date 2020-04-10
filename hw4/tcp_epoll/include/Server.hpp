#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include <functional>
#include <map>
#include "Connection.hpp"
#include "FileDescriptor.hpp"

namespace Tcp_epoll {

using Callback = std::function<void(Connection&)>;

constexpr size_t DEFAULT_MAX_CONNECTION_NUMBER = 5;
constexpr size_t EPOLL_SIZE = 128;

class Server {
public:
    Server() = default;

    ~Server() noexcept;

    void open(const std::string& address, uint16_t port);
    void set_max_connect(size_t max_connect);

    void create_epoll();

    void add_epoll(int fd, uint32_t events);
    void modify_epoll(int fd, uint32_t events);
    void delete_epoll(int fd, uint32_t events);
    void handle_client(int fd, uint32_t event);

    void accept_clients();
    void event_loop();

    bool is_opened() const;
    void close();

    void erase_client(int key) { clients_.erase(key); };
    int get_epoll_fd() { return epoll_fd_; }

    void init(const std::string& address, uint16_t port, Callback handler);

private:
    FileDescriptor fd_;
    FileDescriptor epoll_fd_;

    size_t max_connect_ = DEFAULT_MAX_CONNECTION_NUMBER;
    bool is_opened_;

    std::map<int, Connection> clients_{};

    Callback handler_;
};

} // namespace Tcp_epoll


#endif // SERVER_HPP
