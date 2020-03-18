#include <unistd.h>
#include <cstdio>

int main() {
    char buffer[4];
    while (!feof(stdin)) {
        size_t bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
        write(STDOUT_FILENO, buffer, sizeof(buffer));
    }

    return 0;
}