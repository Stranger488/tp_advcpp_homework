#include <stdexcept>
#include <iostream>
#include "Connection.hpp"

int main() {
    try {
        Tcp::Connection connection("127.0.0.2", 8888);
        std::cout << "Connected." << std::endl;

        connection.set_timeout(10);

        // test big string
        const size_t big_string_size = 100;
        std::string big_string_write(big_string_size, 'a');
        std::string big_string_read(big_string_size, '\0');
        connection.writeExact(big_string_write.c_str(), big_string_size);
        connection.readExact(big_string_read.data(), big_string_size);
        std::cout << big_string_read << std::endl;

        // echo test
        std::string buf_string_in;
        std::string buf_string_out;
        while(std::cin >> buf_string_in) {
            connection.write(buf_string_in.c_str(), buf_string_in.size());
            connection.read(buf_string_out.data(), buf_string_out.size());

            std::cout << buf_string_out << std::endl;
            buf_string_in.clear();
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
