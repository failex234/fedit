#include "fedit.h"


char readKey() {
	int nread;
	char c;
	
	//Read key from stdin
	while((nread = read(STDOUT_FILENO, &c, 1)) != 1) {
		if (nread == -1 && erno != EAGAIN) {
			die("read");
		}
	}
	
	return c;
}

void processKeyPress() {
	char c = readKey();
	
	switch(c) {
		case CTRL_KEY('q'):
			//Erase screen
			write(STDOUT_FILENO, "\x1b[2J", 4);
			//Place cursor to default (1,1) position
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
	}
}