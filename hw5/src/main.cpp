#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "Map.hpp"

constexpr size_t MMAP_NUMBER = 512;

int main() {
    shmem::Map<int, std::string> map(MMAP_NUMBER);

    int pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid != 0) { // parent
        map.insert( {1, "one"} );
        map.insert({3, "three"});
        map.insert({5, "five"});
    }
    else { // child
        map.insert( {2, "two"} );
        map.insert({4, "four"});
        map.insert({6, "six"});

        return 0;
    }

    int status = 0;
    wait(&status);

    for (size_t i = 1; i < 7; i++) {
        std::cout << map.at(i) << std::endl;
    }

    map.destroy();

    return 0;
}
