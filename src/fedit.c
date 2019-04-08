#include "fedit.h"

int main(int argc, char **argv) {
	enableRawMode();
	init();
	if (argc >= 2) {
		file_open(argv[1]);
	}
	
	setStatusMessage("HELP: Ctrl+S = save | Ctrl+Q = quit | Ctrl+F = find");

	while (1) {
		refreshScreen();
		processKeyPress();
	}
	return 0;
}

void init() {
	//Set initial values
	E.cx = 0;
	E.cy = 0;
	E.rx = 0;
	E.rowoff = 0;
	E.coloff = 0;
	E.numrows = 0;
	E.row = NULL;
	E.modified = 0;
	E.filename = NULL;
	E.statusmsg[0] = '\0';
	E.statusmsg_time = 0;
	E.syntax = NULL;
	
	if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
		die("getWindowSize");
	}
	
	E.screenrows -= 2;
}

void moveCursor(int key) {
	erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	switch(key) {
		case ARROW_LEFT:
			if (E.cx != 0) {
				E.cx--;
			} else if (E.cy > 0) {
				E.cy--;
				E.cx = E.row[E.cy].size;
			}
			break;
		case ARROW_RIGHT:
			if (row && E.cx < row->size) {
				E.cx++;
			} else if (row && E.cx == row->size) {
				E.cy++;
				E.cx = 0;
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
	
	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	
	if (E.cx > rowlen) {
		E.cx = rowlen;
	}
}

void setStatusMessage(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	
	vsnprintf(E.statusmsg, sizeof(E.statusmsg), format, ap);
	
	va_end(ap);
	
	E.statusmsg_time = time(NULL);
}