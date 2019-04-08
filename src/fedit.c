#include "fedit.h"

int main(int argc, char **argv) {
	enableRawMode();
	init();
	if (argc >= 2) {
		file_open(argv[1]);
	}

	while (1) {
		refreshScreen();
		processKeyPress();
	}
	return 0;
}

void init() {
	E.cx = 0;
	E.cy = 0;
	E.rowoff = 0;
	E.numrows = 0;
	E.row = NULL;
	if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
		die("getWindowSize");
	}
}

void moveCursor(int key) {
	switch(key) {
		case ARROW_LEFT:
			if (E.cx != 0) {
				E.cx--;
			}
			break;
		case ARROW_RIGHT:
			if (E.cy != E.screencols - 1) {
				E.cx++;
			}
			break;
		case ARROW_UP:
			if (E.cy != 0) {
				E.cy--;
			}
			break;
		case ARROW_DOWN:
			if (E.cy < E.numrows) {
				E.cy++;
			}
			break;
	}
}
