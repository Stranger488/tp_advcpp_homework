#include <iostream>
#include "Server.hpp"
#include "Connection.hpp"

int main() {
    try {
        Tcp::Server server("127.0.0.1", 8888);
        Tcp::Connection connection = server.accept();

        std::cout << "Connection accepted." << std::endl;

        connection.set_timeout(10);

        // test big string
        const size_t big_string_size = 100;
        std::string big_string(big_string_size, '\0');
        connection.readExact(big_string.data(), big_string_size);
        connection.writeExact(big_string.c_str(), big_string_size);

        // echo test
        std::string buf_string;
        while (server.is_opened()) {
            size_t buf_size = buf_string.size();

            connection.read(buf_string.data(), buf_size);
            connection.write(buf_string.c_str(), buf_size);
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}