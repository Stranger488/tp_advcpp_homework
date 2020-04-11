#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include <string>
#include <arpa/inet.h>

namespace Tcp {

enum class SocketTimeOutType{
    SEND_TIMEOUT = SO_SNDTIMEO,
    RECEIVE_TIMEOUT = SO_RCVTIMEO
};

class Connection {
public:
    Connection(const std::string& address, uint16_t port);
    Connection(const std::string& dst_addr, uint16_t dst_port,
               int sock_fd);
    ~Connection() noexcept;

    void connect(const std::string& address, uint16_t port);
    void set_timeout(time_t time, SocketTimeOutType timeout_type);

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);

    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool is_opened() const;

    void close();

private:
    int fd_;

    std::string dst_addr_;
    uint16_t dst_port_;

    std::string src_addr_;
    uint16_t src_port_;

    bool is_opened_;
};

} // namespace Tcp


#endif // CONNECTION_HPP
