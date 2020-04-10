#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include <arpa/inet.h>
#include <vector>
#include "FileDescriptor.hpp"

namespace Tcp_epoll {

class Connection {
public:
    Connection(const std::string& address, uint16_t port);
    Connection(const std::string& dst_addr, uint16_t dst_port,
               FileDescriptor sock_fd);
    Connection(Connection&& other);
    ~Connection() noexcept;

    void connect(const std::string& address, uint16_t port);

    ssize_t write(const void* data, size_t len);
    ssize_t read(void* data, size_t len);


    bool is_opened() const;

    void close();

    int get_fd_() const { return fd_; }
    uint32_t get_event() const { return event_; }
    size_t get_length() const { return length_; }
    size_t get_offset() const { return offset_; }

    void set_length(size_t length) { length_ = length; }
    void set_offset(size_t offset) { length_ = offset_; }
    void set_event(uint32_t event) { event_ = event; }

private:
    FileDescriptor fd_;

    std::string dst_addr_;
    uint16_t dst_port_;

    std::string src_addr_;
    uint16_t src_port_;

    bool is_opened_;


    uint32_t event_;

    size_t length_;
    size_t offset_;
};

} // namespace Tcp_epoll


#endif // CONNECTION_HPP
