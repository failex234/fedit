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
	//Allocate space for the user-entered text
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while(1) {
		//Set the status message to the given format string and the user-entered text
		setStatusMessage(string, buf);
		refreshScreen();

		int c = readKey();
		//Remove the last character
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
			if (buflen != 0) {
				buf[--buflen] = '\0';
			}
		//Abort prompt when hitting the escape key
		} else if (c == '\x1b') { 
			setStatusMessage("");
			free(buf);

			return NULL;
		//Confirm the entered text when hitting return
		} else if (c == '\r') {
			if (buflen != 0) {
				setStatusMessage("");
				return buf;
			}
		//Make sure, that we only deal with "displayable" characters
		} else if (!iscntrl(c) && c < 128) {
			//Resize buffer and add new character to the buffer
			if (buflen == bufsize - 1) {
				bufsize *= 2;
				buf = realloc(buf, bufsize);
			}
			buf[buflen++] = c;
			buf[buflen] = '\0';
		}
	}
}

void find() {
	char *query = prompt("(ESC to cancel) Find: %s");
	if (query == NULL) {
		return;
	}

	for (int i = 0; i < E.numrows; i++) {
		erow *row = &E.row[i];

		char *match = strstr(row->render, query);

		if (match) {
			E.cy = i;
			E.cx = rowRxToCx(row, match - row->render);
			E.rowoff = E.numrows;

			break;
		}
	}
}