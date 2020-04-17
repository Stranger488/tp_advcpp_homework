#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "Map.hpp"

int main() {
    Shmem::Map<int, std::basic_string<char, std::char_traits<char>>> map{};

    int pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid != 0) { // parent
        map.insert({1, "1"});
        map.insert({3, "3"});
        map.insert({5, "5"});
    }
    else { // child
        map.insert({2, "2"});
        map.insert({4, "4"});
        map.insert({6, "6"});

        return 0;
    }

    int status = 0;
    wait(&status);

    for (auto& every : map) {
        std::cout << every.first << " " << every.second << std::endl;
    }

    return 0;
}
