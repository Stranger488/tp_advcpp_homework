#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include "Connection.hpp"

namespace Tcp {

static const int DEFAULT_MAX_CONNECTION_NUMBER = 2;

class Server {
public:
    Server(const std::string& address, uint16_t port);
    ~Server() noexcept;

    void open(const std::string& address, uint16_t port);
    Connection accept();
    void set_max_connect(int max_connect);

    bool is_opened() const;
    void close();

private:
    int fd_;
    int max_connect_ = DEFAULT_MAX_CONNECTION_NUMBER;

    bool is_opened_;
};

} // namespace Tcp


#endif // SERVER_HPP
