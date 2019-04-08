#include "fedit.h"

int main() {
	enableRawMode();
	init();

	while (1) {
		clearScreen();
		processKeyPress();
	}
	return 0;
}

void init() {
	if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
		die("getWindowSize");
	}
}
