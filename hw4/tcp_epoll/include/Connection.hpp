#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include <arpa/inet.h>
#include <vector>
#include <memory>
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
    size_t get_offset_read() const { return offset_read_; }
    size_t get_offset_write() const { return offset_write_; }
    size_t get_offset() const { return offset_; }
    char* get_buf_data() { return buf_.data(); }
    const char* get_buf_c_str() { return buf_.c_str(); }

    void set_offset_read(size_t offset) { offset_read_ = offset ; }
    void set_offset_write(size_t offset) { offset_write_ = offset ; }
    void set_offset(size_t offset) { offset_ = offset ; }
    void set_event(uint32_t event) { event_ = event; }

private:
    FileDescriptor fd_;

    std::string dst_addr_;
    uint16_t dst_port_;

    std::string src_addr_;
    uint16_t src_port_;

    bool is_opened_;

    uint32_t event_;

    std::string buf_;
    size_t offset_read_;
    size_t offset_write_;
    size_t offset_;
};

} // namespace Tcp_epoll


#endif // CONNECTION_HPP
