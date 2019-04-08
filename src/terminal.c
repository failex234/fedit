#include "fedit.h"

//Clear the screen, draw the rows and position the cursor at the top-left
void refreshScreen() {
	scroll();
	
	struct abuf ab = ABUF_INIT;
	
	//Hide the cursor so no "flickering" can occur (Set Mode)
	abAppend(&ab, "\x1b[?25l", 6);
	//Place cursor to default (1,1) position (Cursor Position)
	abAppend(&ab, "\x1b[H", 3);
	
	drawRows(&ab);
	drawStatusBar(&ab);
	drawMessageBar(&ab);
	
	//Place the cursor to the position from the config
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, (E.rx - E.coloff) + 1);
	abAppend(&ab, buf, strlen(buf));
	
	//Show the cursor again (Reset Mode)
	abAppend(&ab, "\x1b[?25h", 6);
	
	//Write the buffer to the screen
	write(STDOUT_FILENO, ab.str_buf, ab.len);
	abFree(&ab);
}

void drawRows(struct abuf *ab) {
	//Place a tilde at the beginning of every line
	for (int i = 0; i < E.screenrows; i++) {
		int filerow = i + E.rowoff;
		if (filerow >= E.numrows) {
			//Display welcome message
			if (E.numrows == 0 && i == E.screenrows / 2) {
				int welcomelen = snprintf(welcome, sizeof(welcome),
					"FEdit -- version %s", FEDIT_VERSION);
				
				//Truncate the welcome message if longer than the number of columns
				if (welcomelen > E.screencols) {
					welcomelen = E.screencols;
				}
			
				//Calculate padding to center the welcome message
				int padding = (E.screencols - welcomelen) / 2;
				if (padding) {
					abAppend(ab, "~", 1);
					padding--;
				}
			
				//Add padding to the left side
				while (padding--) {
					abAppend(ab, " ", 1);
				}
			
				//Display welcome message
				abAppend(ab, welcome, welcomelen);
			} else {
				//Draw tilde in line
				abAppend(ab, "~", 1);
			}
		} else {
			int len = E.row[filerow].rsize - E.coloff;
			if (len < 0) len = 0;
			if (len > E.screencols) len = E.screencols;

			//Character of the line
			char *c = &E.row[filerow].render[E.coloff];

			//The characters' corresponding highlight
			unsigned char *hl = &E.row[filerow].hl[E.coloff];

			int current_color = -1;

			//Get the color of each highlight and print
			//the corresponding escape sequence with it
      		for (int i = 0; i < len; i++) {
				if (iscntrl(c[i])) {
					char sym = (c[i] <= 26) ? '@' + c[i] : '?';
					abAppend(ab, "\x1b[7m", 4);
					abAppend(ab, &sym, 1);
					abAppend(ab, "\x1b[m", 3);

					//Restore the "old" color
					if (current_color != -1) {
						char buf[16];
						int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", current_color);
						abAppend(ab, buf, clen);
					}
				} else if (hl[i] == HL_NORMAL) {
					if (current_color != -1) {
          				abAppend(ab, "\x1b[39m", 5);
						current_color = -1;
					}
          			abAppend(ab, &c[i], 1);
        		} else {
          			int color = syntaxToColor(hl[i]);
					if (color != current_color) {
						current_color = color;
          				char buf[16];

						//Input color code into escape sequence
          				int clen = snprintf(buf, sizeof(buf), "\x1b[%dm", color);
          				abAppend(ab, buf, clen);
					}
          			abAppend(ab, &c[i], 1);
        		}
      		}
			abAppend(ab, "\x1b[39m", 5);
		}
		
		//Clear current line (Erase In Line)
		abAppend(ab, "\x1b[K", 3);
		//Write a carriage-return and a newline in every line except for the last one
		abAppend(ab, "\r\n", 2);
	}
}

//Use ioctl with TIOCGWINSZ to get the current terminal size
//If ioctl fails, set the cursor position the the bottom right (999, 999)
//And get the position of the cursor to determine rows and columns
int getWindowSize(int *rows, int *cols) {
	struct winsize ws;
	
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
			return -1;
		}
		//Use fallback function when ioctl fails
		return getCursorPosition(rows, cols);
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
	}
	
	return 0;
}

int getCursorPosition(int *rows, int *cols) {
	char buf[32];
	unsigned int i = 0;
	
	//"Send" out the escape sequence for cursor position report (Device Status Report)
	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
	
	//Read in the response escape sequence char by char
	//And exit the reading process when we encounter a R
	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, &buf[i], 1) != 1) {
			break;
		}
		if (buf[i] == 'R') {
			break;
		}
		i++;
	}
	//null-terminate the string
	buf[i] = '\0';
	
	//Make sure, that the string starts with the escape characters
	if (buf[0] != '\x1b' || buf[1] != '[') {
		return -1;
	}
	
	//Try to parse the response numbers
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
		return -1;
	}
	
	return 0;
}

void scroll() {
	E.rx = 0;
	
	if (E.cy < E.numrows) {
		E.rx = rowCxToRx(&E.row[E.cy], E.cx);
	}
	
	//When cursor is above the row offset
	if (E.cy < E.rowoff) {
		E.rowoff = E.cy;
	}
	
	//When cursor goes "offscreen" down
	if (E.cy >= E.rowoff + E.screenrows) {
		E.rowoff = E.cy - E.screenrows + 1;
	}
	
	//When cursor is left of the column offset
	if (E.rx < E.coloff) {
		E.coloff = E.rx;
	}
	
	//When cursor goes offscreen to the right
	if (E.rx >= E.coloff + E.screencols) {
		E.coloff = E.rx - E.screencols + 1;
	}
}

void drawStatusBar(struct abuf *ab) {
	//Invert colors (Select Graphic Rendition)
	abAppend(ab, "\x1b[7m", 4);
	char status[80], rstatus[80];
	
	int len = snprintf(status, sizeof(status), "%s %.20s - %d lines", E.modified ? "(modified)" : "", E.filename ? E.filename : "[No Name]", E.numrows);
	int rlen = snprintf(rstatus, sizeof(rstatus), "(%s) %d/%d - %d", E.syntax ? E.syntax->filetype : "Plain Text", E.cy + 1 > E.numrows ? E.cy : E.cy + 1, E.numrows, E.cx + 1);
	
	if (len > E.screencols) {
		len = E.screencols;
	}
	
	abAppend(ab, status, len);
	
	while (len < E.screencols) {
		if (E.screencols - len == rlen) {
			abAppend(ab, rstatus, rlen);
			break;
		} else {
			abAppend(ab, " ", 1);
			len++;
		}
	}
	//Clear all attributes (Select Graphic Rendition)
	abAppend(ab, "\x1b[m", 3);
	abAppend(ab, "\r\n", 2);
}

void drawMessageBar(struct abuf *ab) {
	abAppend(ab, "\x1b[K", 3);
	
	int msglen = strlen(E.statusmsg);
	
	if (msglen > E.screencols) {
		msglen = E.screencols;
	}
	if (msglen && time(NULL) - E.statusmsg_time < 5) {
		abAppend(ab, E.statusmsg, msglen);
	}
}