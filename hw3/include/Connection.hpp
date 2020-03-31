#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include "Socket.hpp"

namespace Tcp {

class Connection {
public:
    Connection(const std::string& address, uint16_t port);
    ~Connection() noexcept;

    void connect(const std::string& address, uint16_t port);
    void set_timeout(time_t time);

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);

    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool is_opened() const;
    void close();

private:
    Socket fd_;

    std::string dst_addr_;
    uint16_t dst_port_;

    std::string src_addr_;
    uint16_t src_port_;

    bool is_opened_;
};

} // namespace Tcp


#endif // CONNECTION_HPP
