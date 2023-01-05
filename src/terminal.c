#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "terminal_io.h"

void abAppend(struct screen_buffer *ab, const char *s, int len) {
    //Reallocate memory for the new string that should get appended
    char *new = realloc(ab->str_buf, ab->len + len);
    if (new == NULL) return;

    //Copy the new string behind the "old" string
    memcpy(&new[ab->len], s, len);

    //Update the struct
    ab->str_buf = new;
    ab->len += len;
}
void abFree(struct screen_buffer *ab) {
    free(ab->str_buf);
}

//Clear the screen, draw the rows and position the cursor at the top-left
void refreshScreen() {
    scroll();

    struct screen_buffer ab = SCREENBUF_INIT;

    //Hide the cursor so no "flickering" can occur (Set Mode)
    abAppend(&ab, "\x1b[?25l", 6);
    //Place cursor to default (1,1) position (Cursor Position)
    abAppend(&ab, "\x1b[H", 3);

    drawRows(&ab);
    drawStatusBar(&ab);
    drawMessageBar(&ab);

    //Place the cursor to the position from the config
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (editorState.cursor_y - editorState.rowoff) + 1, (editorState.cursor_rendered_x - editorState.coloff) + 1);
    abAppend(&ab, buf, strlen(buf));

    //Show the cursor again (Reset Mode)
    abAppend(&ab, "\x1b[?25h", 6);

    //Write the buffer to the screen
    write(STDOUT_FILENO, ab.str_buf, ab.len);
    abFree(&ab);
}

void drawRows(struct screen_buffer *ab) {
    //Place a tilde at the beginning of every line
    for (int i = 0; i < editorState.screenrows; i++) {
        int filerow = i + editorState.rowoff;
        if (filerow >= editorState.numrows) {
            //Display welcome message
            if (editorState.numrows == 0 && i == editorState.screenrows / 3) {
                int welcomelen = snprintf(welcome, sizeof(welcome),
                                          "FEdit -- version %s", FEDIT_VERSION);

                //Truncate the welcome message if longer than the number of columns
                if (welcomelen > editorState.screencols) {
                    welcomelen = editorState.screencols;
                }

                //Calculate padding to center the welcome message
                int padding = (editorState.screencols - welcomelen) / 2;
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
            //Add line number to the beginning
            if (!editorState.disable_linenums) {
                uint linenum = filerow + 1;

                uint max_digitcount = countDigits(editorState.numrows);
                uint line_digitcount = countDigits(linenum);

                char *line_prefix = (char*) malloc(max_digitcount + 1);

                sprintf(line_prefix, "%d", linenum);

                for (uint j = line_digitcount; j < max_digitcount; j++) {
                    line_prefix[j] = ' ';
                }
                line_prefix[max_digitcount] = ' ';

                abAppend(ab, line_prefix, max_digitcount + 1);
            }

            int len = editorState.rows[filerow].render_length - editorState.coloff;
            if (len < 0) len = 0;
            if (len > editorState.screencols) len = editorState.screencols;

            //Character of the line
            char *c = &editorState.rows[filerow].render[editorState.coloff];

            //The characters' corresponding highlight
            unsigned char *hl = &editorState.rows[filerow].highlight_types[editorState.coloff];

            int current_color = -1;

            //Get the color of each highlight and print
            //the corresponding escape sequence with it
            for (int i = 0; i < len; i++) {
                unsigned char hl_curr = editorState.disable_highlight ? HL_NORMAL : hl[i];
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
                } else if (hl_curr == HL_NORMAL) {
                    if (current_color != -1) {
                        abAppend(ab, "\x1b[39m", 5);
                        current_color = -1;
                    }
                    abAppend(ab, &c[i], 1);
                } else {
                    int color = syntaxToColor(hl_curr);
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

//Use ioctl with TIOCGWINSZ to get the current terminal length
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
    editorState.cursor_rendered_x = 0;

    if (editorState.cursor_y < editorState.numrows) {
        editorState.cursor_rendered_x = rowCxToRx(&editorState.rows[editorState.cursor_y], editorState.cursor_x);
    }

    //When cursor is above the rows offset
    if (editorState.cursor_y < editorState.rowoff) {
        editorState.rowoff = editorState.cursor_y;
    }

    //When cursor goes "offscreen" down
    if (editorState.cursor_y >= editorState.rowoff + editorState.screenrows) {
        editorState.rowoff = editorState.cursor_y - editorState.screenrows + 1;
    }

    //When cursor is left of the column offset
    if (editorState.cursor_rendered_x < editorState.coloff) {
        editorState.coloff = editorState.cursor_rendered_x;
    }

    //When cursor goes offscreen to the right
    if (editorState.cursor_rendered_x >= editorState.coloff + editorState.screencols) {
        editorState.coloff = editorState.cursor_rendered_x - editorState.screencols + 1;
    }
}

void drawStatusBar(struct screen_buffer *ab) {
    //Invert colors (Select Graphic Rendition)
    abAppend(ab, "\x1b[7m", 4);
    char status[80], rstatus[80];

    int len = snprintf(status, sizeof(status), "%s %.20s - %d line%s", editorState.modified ? "(modified)" : "", editorState.filename ? editorState.filename : "[No Name]", editorState.numrows, editorState.numrows == 1 ? "" : "s");
    int rlen = snprintf(rstatus, sizeof(rstatus), "(%s) %d/%d - %d", editorState.syntax ? editorState.syntax->pretty_name : "Plain Text", editorState.cursor_y + 1 > editorState.numrows ? editorState.cursor_y : editorState.cursor_y + 1, editorState.numrows, editorState.cursor_x + 1);

    if (len > editorState.screencols) {
        len = editorState.screencols;
    }

    abAppend(ab, status, len);

    while (len < editorState.screencols) {
        if (editorState.screencols - len == rlen) {
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

void drawMessageBar(struct screen_buffer *ab) {
    abAppend(ab, "\x1b[K", 3);

    int msglen = strlen(editorState.statusmsg);

    if (msglen > editorState.screencols) {
        msglen = editorState.screencols;
    }
    if (msglen && time(NULL) - editorState.statusmsg_time < 5) {
        abAppend(ab, editorState.statusmsg, msglen);
    }
}

void updateWindowSize() {
    if (getWindowSize(&editorState.screenrows, &editorState.screencols) == -1) {
        die("getWindowsSize");
    }
    editorState.screenrows -= 2;
}

uint countDigits(uint num) {
    uint count = 0;

    while (num > 0) {
        num /= 10;
        count++;
    }
    return count;
}
