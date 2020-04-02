#include <iostream>
#include "Server.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./server ip port" << std::endl;
        return -1;
    }

    try {
        Tcp::Server server(std::string(argv[1]), std::stoi(argv[2]));
        Tcp::Connection connection = server.accept();

        std::cout << "Connection accepted." << std::endl;

        connection.set_timeout(5, Tcp::SocketTimeOutType::SEND_TIMEOUT);
        connection.set_timeout(5, Tcp::SocketTimeOutType::RECEIVE_TIMEOUT);


        // test big string
        const size_t big_string_size = 100;
        std::string big_string(big_string_size, '\0');
        connection.readExact(big_string.data(), big_string_size);
        connection.writeExact(big_string.c_str(), big_string_size);


        // echo test
        std::string buf_string_read(BUFSIZ, '\0');
        size_t buf_string_read_size = buf_string_read.size();
        while (connection.is_opened() && server.is_opened()) {
            size_t read_size = connection.read(buf_string_read.data(), buf_string_read_size);

            connection.write(buf_string_read.c_str(), read_size);
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}