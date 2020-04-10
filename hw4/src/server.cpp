#include <iostream>
#include <sys/epoll.h>
#include <cstring>
#include "Server.hpp"

constexpr int BLOCK_SIZE = 4;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./server ip port" << std::endl;
        return -1;
    }

    Tcp_epoll::Server server;

    auto read_from_client_handler = [&server] (Tcp_epoll::Connection& connection) {
        auto con_fd = connection.get_fd_();
        auto con_offset_read = connection.get_offset_read();
        auto con_offset_write = connection.get_offset_read();
        auto con_buf_data = connection.get_buf_data();

        auto con_offset = connection.get_offset();
        ssize_t bytes = connection.read(con_buf_data + con_offset_read, BLOCK_SIZE);

        if (bytes == -1) {
            std::cerr << strerror(errno) << std::endl;
            server.erase_client(con_fd);
        }
        else if (bytes == 0) {
            server.erase_client(con_fd);
        }
        else if (bytes < BLOCK_SIZE) {
            connection.set_offset_read(con_offset_read + bytes);
            connection.set_offset_write(con_offset_write + bytes);
            connection.set_offset(con_offset + bytes);

            server.modify_epoll(con_fd, EPOLLIN);
        }
        else {
            connection.set_offset_read(con_offset_read + BLOCK_SIZE - con_offset_read % BLOCK_SIZE);
            connection.set_offset(0);

            server.modify_epoll(connection.get_fd_(), EPOLLOUT);
        }
    };

    auto write_to_client_handler = [&server] (Tcp_epoll::Connection& connection) {
        auto con_fd = connection.get_fd_();
        auto con_offset_write = connection.get_offset_write();
        auto con_buf_c_str = connection.get_buf_c_str();

        std::cout << con_buf_c_str << std::endl;

        auto con_offset = connection.get_offset();
        ssize_t bytes = connection.write(con_buf_c_str + con_offset_write, BLOCK_SIZE);

        if (bytes == -1) {
            std::cerr << strerror(errno) << std::endl;
            server.erase_client(con_fd);
        }
        else if (bytes == 0) {
            server.erase_client(con_fd);
        }
        else if (bytes < BLOCK_SIZE) {
            connection.set_offset_read(con_offset_read + bytes);
            connection.set_offset_write(con_offset_write + bytes);
            connection.set_offset(con_offset + bytes);

            server.modify_epoll(con_fd, EPOLLOUT);
        }
        else {
            if (con_offset_write == 0) {
                connection.set_offset_write(con_offset_write + BLOCK_SIZE);
            } else {
                connection.set_offset_write(con_offset_write + BLOCK_SIZE - con_offset_write % BLOCK_SIZE);
            }
            connection.set_offset(0);

            server.modify_epoll(con_fd, EPOLLIN);
        }
    };

    while (true) {
        try {
            server.init(argv[1], std::stoi(argv[2]), read_from_client_handler, write_to_client_handler);
            server.event_loop();
        } catch (std::runtime_error& err) {
            std::cout << err.what() << std::endl;
            server.event_loop();
        }
    }

    return 0;
}
