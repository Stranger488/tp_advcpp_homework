#include <stdexcept>
#include <iostream>

#include "Connection.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./client dst_ip dst_port" << std::endl;
        return -1;
    }

    try {
        Tcp::Connection connection(argv[1], std::stoi(argv[2]));
        std::cout << "Connected." << std::endl;


        // test big string
        const size_t buf_big_string_size = 100;
        std::string buf_big_string_write(buf_big_string_size, 'a');
        std::string buf_big_string_read(buf_big_string_size, '\0');

        connection.writeExact(buf_big_string_write.c_str(), buf_big_string_size);
        connection.readExact(buf_big_string_read.data(), buf_big_string_size);
        std::cout << buf_big_string_read << std::endl;


        // echo test
        std::string buf_string_read;
        std::string buf_string_write;

        while(connection.is_opened() && std::cin >> buf_string_read) {
            size_t written_size = connection.write(buf_string_read.c_str(), buf_string_read.size());
            buf_string_write.resize(written_size);

            connection.read(buf_string_write.data(), written_size);
            std::cout << "Read string: " << buf_string_write << std::endl;

            buf_string_read.clear();
            buf_string_write.clear();
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
