#include <unistd.h>

#include "Map.hpp"

int main() {
    auto map = Shmem::Map<int, int>();

    int pid = fork();
    if (pid < 0) {
        throw std::runtime_error(strerror(errno));
    }
    else if (pid != 0) { // parent
        map.
    }
}