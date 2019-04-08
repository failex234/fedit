#include "fedit.h"


int readKey() {
	int nread;
	char c;
	
	//Read key from stdin
	while((nread = read(STDOUT_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("read");
		}
	}
	
	//Check for control characters
	if (c == '\x1b') {
		char seq[3];
		
		//Make sure, that the input is a escape sequence
		if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) {
			return '\x1b';
		}
		
		//Determine control character
		if (seq[0] == '[') {
			switch (seq[1]) {
				case 'A': return ARROW_UP;
				case 'B': return ARROW_DOWN;
				case 'C': return ARROW_RIGHT;
				case 'D': return ARROW_LEFT;
			}
		}
		
		return '\x1b'; 
	}
	
	return c;
}

void processKeyPress() {
	int c = readKey();
	
	switch(c) {
		case CTRL_KEY('q'):
			//Erase screen
			write(STDOUT_FILENO, "\x1b[2J", 4);
			//Place cursor to default (1,1) position
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			moveCursor(c);
			break;
	}
}