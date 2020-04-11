#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include <arpa/inet.h>

#include "FileDescriptor.hpp"

namespace Tcp_epoll {

constexpr size_t BUFSIZE = 1024;

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
    std::string& get_buf() { return buf_; }
    size_t get_offset_read() const;
    size_t get_offset_write() const;
    size_t get_last_read_bytes() const;

    void set_event(uint32_t event);
    void set_offset_read(size_t offset);
    void set_offset_write(size_t offset);
    void set_last_read_bytes(size_t bytes);

private:
    FileDescriptor fd_;

    std::string dst_addr_;
    uint16_t dst_port_;

    std::string src_addr_;
    uint16_t src_port_;

    bool is_opened_;

    uint32_t event_;
    std::string buf_;
    size_t offset_read_ = 0;
    size_t offset_write_ = 0;
    size_t last_read_bytes_ = 0;
};

} // namespace Tcp_epoll


#endif // CONNECTION_HPP
