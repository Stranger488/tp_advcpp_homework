#include <stdexcept>
#include <iostream>
#include <vector>

#include "process.h"

int main() {
    try {
        const std::string path = "/bin/cat";
        Process process(path);

        std::string temp_string;
        const size_t N = 100000; // big_string size
        while ((std::cin >> temp_string) && process.isWorking()) {
            std::string buf(temp_string.size(), '\0');

            process.writeExact(temp_string.c_str(), temp_string.size());
            process.readExact(buf.data(), buf.size());

            std::cout << buf.data() << std::endl;

            temp_string.clear();


            std::string big_string(N, 'a');
            std::string big_buf(N, '\0');

            process.write(big_string.c_str(), N);
            process.read(big_buf.data(), N);

            std::cout << big_buf.data() << std::endl;
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
