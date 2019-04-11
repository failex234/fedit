#include "fedit.h"
void insertChar(int c) {
	if (E.cy == E.numrows) {
		insertRow(E.numrows, "", 0);
	}
	
	if (c == '{') {
		E.indentNewLine++;
	} else if (c == '}' && E.indentNewLine != 0) {
		E.indentNewLine--;
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
		//Append the text of the current line to the line above and then delete the current line
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
		
		int cx_backup = E.cx;
		int cy_backup = E.cy;
		
		E.cy++;
		
		//Indent the next line, when we typed braces
		for (int i = 0; i < E.indentNewLine; i++) {
			insertChar('\t');
		}
		
		E.cx = cx_backup;
		E.cy = cy_backup;
	}
	E.cy++;
	E.cx = 0;
}

char *prompt(char *string, void (*callback)(char *, int)) {
	//Allocate space for the user-entered text
	size_t bufsize = 128;
	char *buf = malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	if (E.vimEmulation) {
		VIM.mode |= VIM_PROMPT_MODE;
	}

	while(1) {
		//Set the status message to the given format string and the user-entered text
		setStatusMessage(-1, string, buf);
		refreshScreen();

		int c = readKey();
		//Remove the last character
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
			if (buflen != 0) {
				buf[--buflen] = '\0';
			}
		//Abort prompt when hitting the escape key
		} else if (c == '\x1b') { 
			setStatusMessage(0, "");

			if (callback) {
				callback(buf, c);
			}

			free(buf);

			//Exit vim prompt mode
			if (E.vimEmulation) {
				VIM.mode ^= VIM_PROMPT_MODE;
				VIM.mode ^= VIM_SEARCH_MODE;
			}

			return NULL;
		//Confirm the entered text when hitting return
		} else if (c == '\r') {
			if (buflen != 0) {
				setStatusMessage(0, "");

				if (callback) {
					callback(buf, c);
				}

				//Exit vim prompt mode
				if (E.vimEmulation) {
					VIM.mode ^= VIM_PROMPT_MODE;
				}

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

		if (callback) {
			callback(buf, c);
		}
	}
}

void find() {
	int saved_cx = E.cx;
	int saved_cy = E.cy;
	int saved_coloff = E.coloff;
	int saved_rowoff = E.rowoff;

	
	char *query;
	if (E.vimEmulation) {
		query = prompt("/%s", findCallback);
	} else {
		query = prompt("(ESC to cancel) Find: %s", findCallback);
	}

	if (query) {
		free(query);
	} else {
		E.cx = saved_cx;
		E.cy = saved_cy;
		E.coloff = saved_coloff;
		E.rowoff = saved_rowoff;
	}
	
	if (VIM.mode & VIM_SEARCH_MODE) {
		VIM.mode ^= VIM_SEARCH_MODE;
	}
}

void findCallback(char *query, int key) {
	static int last_match = -1;
	static int direction = 1;

	static int saved_hl_line;
	static char *saved_hl = NULL;

	if (saved_hl) {
		memcpy(E.row[saved_hl_line].hl, saved_hl, E.row[saved_hl_line].rsize);
		free(saved_hl);
		saved_hl = NULL;
	}

	if (key == '\r' || key == '\x1b') {
		last_match = -1;
		direction = 1;
		return;
	} else if(key == ARROW_RIGHT || key == ARROW_DOWN) {
		direction = 1;
	} else if (key == ARROW_LEFT || key == ARROW_UP) {
		direction = -1;
	} else {
		last_match = -1;
		direction = 1;
	}

	if (last_match == -1) {
		direction = 1;
	}

	int current = last_match;

	for (int i = 0; i < E.numrows; i++) {
		current += direction;
		if (current == -1) {
			current = E.numrows - 1;
		} else if (current == E.numrows) {
			current = 0;
		}

		erow *row = &E.row[current];

		char *match = strstr(row->render, query);

		if (match) {
			last_match = current;
			E.cy = current;
			E.cx = rowRxToCx(row, match - row->render);
			E.rowoff = E.numrows;

			//Save all highlightings of the search query
			saved_hl_line = current;
			saved_hl = malloc(row->rsize);
			memcpy(saved_hl, row->hl, row->rsize);

			//Set the color to HL_MATCH
			memset(&row->hl[match - row->render], HL_MATCH, strlen(query));

			break;
		}
	}
}

void quit() {
	//Erase screen
	write(STDOUT_FILENO, "\x1b[2J", 4);
	//Place cursor to default (1,1) position
	write(STDOUT_FILENO, "\x1b[H", 3);
	exit(0);
}