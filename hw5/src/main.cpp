#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "Map.hpp"

constexpr size_t MMAP_NUMBER = 1024;

int main() {
    shmem::Map< int, std::basic_string<char, std::char_traits<char>> > map(MMAP_NUMBER);

    int pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid != 0) { // parent
        map.insert({1, "one"});
        map.insert({3, "three"});
        map.insert({5, "five"});
    }
    else { // child
        map.insert({2, "two"});
        map.insert({4, "four"});
        map.insert({6, "six"});

        return 0;
    }

    int status = 0;
    wait(&status);

    for (auto& every : map) {
        std::cout << every.first << " " << every.second << std::endl;
    }

    return 0;
}
