#include "fedit.h"
void insertChar(int c) {
	if (E.cy == E.numrows) {
		insertRow(E.numrows, "", 0);
	}
	rowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}

void deleteChar() {
	if (E.cy == E.numrows || (E.cx == 0 && E.cy == 0)) {
		return;
	}

	erow *row = &E.row[E.cy];
	if (E.cx > 0) {
		rowDeleteChar(row, E.cx - 1);
		E.cx--;
	} else {
		//Append the text of the current line to the line above and then delete the current lin
		E.cx = E.row[E.cy - 1].size;

		rowAppendString(&E.row[E.cy - 1], row->chars, row->size);
		deleteRow(E.cy);

		E.cy--;
	}
}

void insertNewLine() {
	if (E.cx == 0) {
		//insert a blank line when we're at the beginning of a line
		insertRow(E.cy, "", 0);
	} else {
		erow *row = &E.row[E.cy];

		insertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);

		//Update the new row
		row = &E.row[E.cy];
		row->size = E.cx;
		row->chars[row->size] = '\0';

		updateRow(row);
	}
	E.cy++;
	E.cx = 0;
}

char *prompt(char *string) {
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while(1) {
		setStatusMessage(string, buf);
		refreshScreen();

		int c = readKey();
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
			if (buflen != 0) {
				buf[--buflen] = '\0';
			}
		} else if (c == '\x1b') { 
			setStatusMessage("");
			free(buf);

			return NULL;
		} else if (c == '\r') {
			if (buflen != 0) {
				setStatusMessage("");
				return buf;
			}
		} else if (!iscntrl(c) && c < 128) {
			if (buflen == bufsize - 1) {
				bufsize *= 2;
				buf = realloc(buf, bufsize);
			}
			buf[buflen++] = c;
			buf[buflen] = '\0';
		}
	}
}