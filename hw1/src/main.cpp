#include <stdexcept>
#include <iostream>
#include <vector>

#include "process.h"

int main() {
    try {
        const std::string path = "/bin/cat";
        Process process(path);

        std::string temp_string;
        while ((std::cin >> temp_string) && process.isWorking()) {
            std::vector<char> buf(temp_string.size());

            process.writeExact(temp_string.c_str(), temp_string.size());
            process.readExact(buf.data(), buf.size());

            std::cout << buf.data() << std::endl;

            temp_string.clear();
        }

        process.closeStdout();
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
