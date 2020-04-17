#include <iostream>
#include <sys/epoll.h>
#include <cstring>
#include "Server.hpp"

constexpr int BLOCK_SIZE = 256;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./server ip port" << std::endl;
        return -1;
    }

    tcp_epoll::Server server;

    auto read_from_client_handler = [&server] (tcp_epoll::Connection& connection) {
        auto con_fd = connection.get_fd_();
        auto con_buf_data = connection.get_buf().data();
        auto con_offset_read = connection.get_offset_read();

        ssize_t bytes = connection.read(con_buf_data + con_offset_read, BLOCK_SIZE);
        connection.set_last_read_bytes(bytes);

        if (bytes == -1) {
            std::cerr << strerror(errno) << std::endl;
            server.erase_client(con_fd);
        }
        else if (bytes == 0) {
            server.erase_client(con_fd);
        }
        else {
            connection.set_offset_read(con_offset_read + bytes);
            server.modify_epoll(connection.get_fd_(), EPOLLOUT);
        }
    };

    auto write_to_client_handler = [&server] (tcp_epoll::Connection& connection) {
        auto con_fd = connection.get_fd_();
        auto con_buf_c_str = connection.get_buf().c_str();
        auto con_offset_write = connection.get_offset_write();
        auto con_last_read_bytes = connection.get_last_read_bytes();

        ssize_t bytes = connection.write(con_buf_c_str + con_offset_write, con_last_read_bytes);

        if (bytes == -1) {
            std::cerr << strerror(errno) << std::endl;
            server.erase_client(con_fd);
        }
        else if (bytes == 0) {
            server.erase_client(con_fd);
        }
        else {
            connection.set_offset_write(con_offset_write + bytes);
            server.modify_epoll(con_fd, EPOLLIN);
        }
    };

    std::string address = argv[1];
    uint16_t port = std::stoi(argv[2]);
    while (true) {
        try {
            server.init(address, port, read_from_client_handler, write_to_client_handler);
            server.event_loop();
        } catch (std::runtime_error& err) {
            std::cout << err.what() << std::endl;
            server.event_loop();
        }
    }

    return 0;
}
