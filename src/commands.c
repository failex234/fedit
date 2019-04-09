#include "fedit.h"

void parseCommandLine(const char *command) {
    if (!strcmp(command, "q")) {
        //Erase screen
		write(STDOUT_FILENO, "\x1b[2J", 4);
		//Place cursor to default (1,1) position
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
    } else if (!strcmp(command, "w")) {
        file_save();
    }
}