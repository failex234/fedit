#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void die(const char *string) {
    //Erase display (Erase In Display)
    write(STDOUT_FILENO, "\x1b[2J", 4);
    //Place cursor to default (1,1) position (Cursor Position)
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(string);
    exit(1);
}
