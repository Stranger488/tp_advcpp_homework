#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <iostream>

#include "process.h"

int main() {
    try {
        const std::string path = "../tests/test_hw1";
        Process process(path);

        process.write(path.c_str(), path.length());

        char buf[path.length()];

        void *buf_ptr = buf;
        process.read(buf, path.length());
        buf[path.length() - 1] = '\n';

        std::cout << buf << std::endl;
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
