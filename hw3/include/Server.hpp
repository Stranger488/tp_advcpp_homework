#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include "Connection.hpp"

namespace Tcp {

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
};

} // namespace Tcp


#endif // SERVER_HPP
