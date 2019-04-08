#include "fedit.h"

int main() {
	enableRawMode();
	init();

	while (1) {
		refreshScreen();
		processKeyPress();
	}
	return 0;
}

void init() {
	E.cx = 0;
	E.cy = 0;
	if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
		die("getWindowSize");
	}
}

void moveCursor(int key) {
	switch(key) {
		case ARROW_LEFT:
			E.cx--;
			break;
		case ARROW_RIGHT:
			E.cx++;
			break;
		case ARROW_UP:
			E.cy--;
			break;
		case ARROW_DOWN:
			E.cy++;
			break;
	}
}
