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
			if (seq[1] >= '0' && seq[1] <= '9') {
				if (read(STDIN_FILENO, &seq[2], 1) != 1) {
					return '\x1b';
				}
				
				if (seq[2] == '~') {
					switch(seq[1]) {
						case '1': return HOME_KEY;
						case '3': return DEL_KEY;
						case '4': return END_KEY;
						case '5': return PAGE_UP;
						case '6': return PAGE_DOWN;
						case '7': return HOME_KEY;
						case '8': return END_KEY;
					}
				}
			} else {
				switch (seq[1]) {
					case 'A': return ARROW_UP;
					case 'B': return ARROW_DOWN;
					case 'C': return ARROW_RIGHT;
					case 'D': return ARROW_LEFT;
					case 'H': return HOME_KEY;
					case 'F': return END_KEY;
				}
			}
		} else if (seq[0] == 'O') {
			switch(seq[1]) {
				case 'H': return HOME_KEY;
				case 'F': return END_KEY;
			}
		}
		return '\x1b'; 
	}
	return c;
}

//Does stuff when certain keys are pressed
void processKeyPress() {
	static int quit_times = FEDIT_QUIT_TIMES;

	int c = readKey();
	
	switch(c) {
		case '\r':
			insertNewLine();
			break;
		case CTRL_KEY('q'):
			if (E.modified && quit_times > 0) {
				setStatusMessage("Warning! Your file has unsaved changes! Press Ctrl+Q %d more time%s to quit.", quit_times, quit_times > 1 ? "s" : "");
				quit_times--;
				return;
			}
			//Erase screen
			write(STDOUT_FILENO, "\x1b[2J", 4);
			//Place cursor to default (1,1) position
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;
		case CTRL_KEY('s'):
			file_save();
			break;
		case CTRL_KEY('f'):
			find();
			break;
		case HOME_KEY:
			E.cx = 0;
			break;
		case END_KEY:
			//Place the cursor at the end of the line
			if (E.cy < E.numrows) {
				E.cx = E.row[E.cy].size;
			}
			break;
		case BACKSPACE:
		case CTRL_KEY('h'):
		case DEL_KEY:
			if (c == DEL_KEY) {
				moveCursor(ARROW_RIGHT);
			}
			deleteChar();
			break;
		case PAGE_UP:
		case PAGE_DOWN:
			{
				if (c == PAGE_UP) {
					E.cy = E.rowoff;
				} else if (c == PAGE_DOWN) {
					E.cy = E.rowoff + E.screenrows - 1;
					
					if (E.cy > E.numrows) {
						E.cy = E.numrows;
					}
					
					int times = E.screenrows;
					while(times--) {
						moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
					}
				}
				break;
			}
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			moveCursor(c);
			break;
		case CTRL_KEY('l'):
		case '\x1b':
			break;
		default:
			insertChar(c);
			break;
	}
	quit_times = FEDIT_QUIT_TIMES;
}

void handleSigWinch(int signal) {
	updateWindowSize();
	
	if (E.cy > E.screenrows) E.cy = E.screenrows - 1;
	if (E.cx > E.screencols) E.cx = E.screencols - 1;
	
	refreshScreen();
}