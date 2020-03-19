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


            std::string big_string(1e5, 'a');
            std::vector<char> big_buf(big_string.size());

            process.write(big_string.c_str(), big_string.size());
            process.read(big_buf.data(), big_buf.size());

            std::cout << big_buf.data() << std::endl;
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
