#include <iostream>
#include "Server.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Invalid program arguments: usage ./server ip port" << std::endl;
        return -1;
    }

    try {

    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
