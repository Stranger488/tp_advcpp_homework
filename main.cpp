#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int fildes[2];
    const int BSIZE = 100;
    char buf[BSIZE];
    ssize_t nbytes;
    int status;

    status = pipe(fildes);
    if (status == -1) {
        /* an error occurred */

    }

    switch (fork()) {
        case -1: /* Handle error */
            break;

        case 0:  /* Child - reads from pipe */
            dup2(fildes[0], STDIN_FILENO);

            close(fildes[0]);
            close(fildes[1]);                       /* Write end is unused */

            nbytes = read(STDIN_FILENO, buf, BSIZE);   /* Get data from pipe */

            execl("/bin/cat", "cat", NULL);

            exit(EXIT_SUCCESS);

        default:  /* Parent - writes to pipe */
            close(fildes[0]);                       /* Read end is unused */
            write(fildes[1], "Hello world\n", 12);  /* Write data on pipe */
            close(fildes[1]);                       /* Child will see EOF */

            while ((wait(&status)) != (-1)) {
                usleep(3600);
            }

            exit(EXIT_SUCCESS);
    }

    return 0;
}
