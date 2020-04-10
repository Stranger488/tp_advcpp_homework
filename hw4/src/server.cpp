#include <iostream>
#include <sys/epoll.h>
#include <cstring>
#include "Server.hpp"

constexpr size_t buf_string_size = 1024;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./server ip port" << std::endl;
        return -1;
    }

    try {
        Tcp_epoll::Server server;

        auto handler = [&server] (Tcp_epoll::Connection& connection) {
            if (connection.get_event() == EPOLLIN) {
                server.delete_epoll(connection.get_fd_(), EPOLLIN);

                std::string buf_string_read(BUFSIZ, '\0');
                ssize_t bytes = connection.read(buf_string_read.data(), buf_string_read.size());

                if ( (bytes == -1 && errno == EINTR) || bytes < connection.get_length()) { //
                    server.modify_epoll(connection.get_fd_(), EPOLLIN);

                    if (bytes != -1) {
                        connection.set_length(connection.get_length() - bytes);
                        connection.set_offset(connection.get_offset() + bytes);
                    }
                }
                if (bytes == 0) { // client disconnected
                    server.erase_client(connection.get_fd_());
                }
                else {
                    connection.set_length(bytes);
                    server.add_epoll(connection.get_fd_(), EPOLLOUT);
                }
            }
            else if (connection.get_event() == EPOLLOUT) {
                server.delete_epoll(connection.get_fd_(), EPOLLOUT);

                std::string buf_string_write(100, 'a');
                ssize_t bytes = connection.write(buf_string_write.c_str(), buf_string_write.size());

                if ( (bytes == -1 && errno == EINTR) || bytes < connection.get_length()) {
                    server.modify_epoll(connection.get_fd_(), EPOLLOUT);

                    if (bytes != -1) {
                        connection.set_length(connection.get_length() - bytes);
                        connection.set_offset(connection.get_offset() + bytes);
                    }
                }
                else if (bytes == -1) {
                    server.erase_client(connection.get_fd_());
                }
                else {
                    server.add_epoll(connection.get_fd_(), EPOLLIN);
                }
            }
        };

        server.init(argv[1], std::stoi(argv[2]), handler);
        server.event_loop();

    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
