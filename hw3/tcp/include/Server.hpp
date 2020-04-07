#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include "Connection.hpp"

namespace Tcp {

static const size_t DEFAULT_MAX_CONNECTION_NUMBER = 2;

class Server {
public:
    Server(const std::string& address, uint16_t port);
    ~Server() noexcept;

    void open(const std::string& address, uint16_t port);
    Connection accept();
    void set_max_connect(size_t max_connect);

    bool is_opened() const;
    void close();

private:
    int fd_;
    size_t max_connect_ = DEFAULT_MAX_CONNECTION_NUMBER;

    bool is_opened_;
};

} // namespace Tcp


#endif // SERVER_HPP
