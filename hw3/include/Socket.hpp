#ifndef SOCKET_HPP
#define SOCKET_HPP


namespace Tcp {

class Socket {
public:
    Socket();
    ~Socket() noexcept;

    int get_fd() const;
    int extract_fd();

    void close();

private:
    int fd_;
};

} // namespace Tcp


#endif // SOCKET_HPP
