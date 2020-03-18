#include <stdexcept>
#include <iostream>
#include <vector>

#include "process.h"

int main() {
    try {
        const std::string path = "/bin/cat";
        Process process(path);

        std::string temp_string("initial");
        while (!temp_string.empty()) {
            temp_string.clear();
            std::cin >> temp_string;

            process.writeExact(temp_string.c_str(), temp_string.size());

            std::vector<char> buf(temp_string.size());
            process.readExact(buf.data(), buf.size());

            std::cout << buf.data() << std::endl;


            const int N = 1e5;
            std::vector<char> buf2(N);
            for (char &i : buf2) {
                i = 'a';
            }
            process.write(buf2.data(), buf2.size());
            process.read(buf2.data(), buf2.size());
            std::cout << buf2.data() << std::endl;
        }
    } catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        return -1;
    }

    return 0;
}
