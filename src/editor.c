#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "editor.h"
#include "highlighting.h"
#include "rows.h"
#include "terminal.h"
#include "terminal_io.h"


void insertChar(int c) {
    if (editorState.cursor_y == editorState.numrows) {
        insertRow(editorState.numrows, "", 0);
    }

    if (c == '{') {
        editorState.indent_newline++;
    } else if (c == '}' && editorState.indent_newline != 0) {
        editorState.indent_newline--;
    }

    rowInsertChar(&editorState.rows[editorState.cursor_y], editorState.cursor_x, c);
    editorState.cursor_x++;
}

void deleteChar() {
    if (editorState.cursor_y == editorState.numrows || (editorState.cursor_x == 0 && editorState.cursor_y == 0)) {
        return;
    }

    struct erow *row = &editorState.rows[editorState.cursor_y];
    if (editorState.cursor_x > 0) {
        rowDeleteChar(row, editorState.cursor_x - 1);
        editorState.cursor_x--;
    } else {
        //Append the text of the current line to the line above and then delete the current line
        editorState.cursor_x = editorState.rows[editorState.cursor_y - 1].length;

        rowAppendString(&editorState.rows[editorState.cursor_y - 1], row->chars, row->length);
        deleteRow(editorState.cursor_y);

        editorState.cursor_y--;
    }
}

void insertNewLine() {
    if (editorState.cursor_x == 0) {
        //insert a blank line when we're at the beginning of a line
        insertRow(editorState.cursor_y, "", 0);
    } else {
        struct erow *row = &editorState.rows[editorState.cursor_y];

        insertRow(editorState.cursor_y + 1, &row->chars[editorState.cursor_x], row->length - editorState.cursor_x);

        //Update the new rows
        row = &editorState.rows[editorState.cursor_y];
        row->length = editorState.cursor_x;
        row->chars[row->length] = '\0';

        updateRow(row);

        int cx_backup = editorState.cursor_x;
        int cy_backup = editorState.cursor_y;

        editorState.cursor_y++;

        //Indent the next line, when we typed braces
        for (int i = 0; i < editorState.indent_newline; i++) {
            insertChar('\t');
        }

        editorState.cursor_x = cx_backup;
        editorState.cursor_y = cy_backup;
    }
    editorState.cursor_y++;
    editorState.cursor_x = 0;
}

char *prompt(char *string, void (*callback)(char *, int)) {
    //Allocate space for the user-entered text
    size_t bufsize = 128;
    char *buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    if (editorState.vim_emulation) {
        vimState.mode |= VIM_PROMPT_MODE;
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
            if (editorState.vim_emulation) {
                vimState.mode ^= VIM_PROMPT_MODE;
                vimState.mode ^= VIM_SEARCH_MODE;
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
                if (editorState.vim_emulation) {
                    vimState.mode ^= VIM_PROMPT_MODE;
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
    int saved_cx = editorState.cursor_x;
    int saved_cy = editorState.cursor_y;
    int saved_coloff = editorState.coloff;
    int saved_rowoff = editorState.rowoff;


    char *query;
    if (editorState.vim_emulation) {
        query = prompt("/%s", findCallback);
    } else {
        query = prompt("(ESC to cancel) Find: %s", findCallback);
    }

    if (query) {
        free(query);
    } else {
        editorState.cursor_x = saved_cx;
        editorState.cursor_y = saved_cy;
        editorState.coloff = saved_coloff;
        editorState.rowoff = saved_rowoff;
    }

    if (vimState.mode & VIM_SEARCH_MODE) {
        vimState.mode ^= VIM_SEARCH_MODE;
    }
}

void findCallback(char *query, int key) {
    static int last_match = -1;
    static int direction = 1;

    static int saved_hl_line;
    static char *saved_hl = NULL;

    if (saved_hl) {
        memcpy(editorState.rows[saved_hl_line].highlight_types, saved_hl, editorState.rows[saved_hl_line].render_length);
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

    for (int i = 0; i < editorState.numrows; i++) {
        current += direction;
        if (current == -1) {
            current = editorState.numrows - 1;
        } else if (current == editorState.numrows) {
            current = 0;
        }

        struct erow *row = &editorState.rows[current];

        char *match = strstr(row->render, query);

        if (match) {
            last_match = current;
            editorState.cursor_y = current;
            editorState.cursor_x = rowRxToCx(row, match - row->render);
            editorState.rowoff = editorState.numrows;

            //Save all highlightings of the search query
            saved_hl_line = current;
            saved_hl = malloc(row->render_length);
            memcpy(saved_hl, row->highlight_types, row->render_length);

            //Set the color to HL_MATCH
            memset(&row->highlight_types[match - row->render], HL_MATCH, strlen(query));

            break;
        }
    }
}

void quit(int status) {
    clear_screen();
    exit(status);
}

void goToLine(int line) {
    editorState.cursor_y = line - 1;
}

void moveCursor(int key) {
    struct erow *row = (editorState.cursor_y >= editorState.numrows) ? NULL : &editorState.rows[editorState.cursor_y];
    switch(key) {
        case ARROW_LEFT:
            if (editorState.cursor_x != 0) {
                editorState.cursor_x--;
            } else if (editorState.cursor_y > 0 && !editorState.vim_emulation) {
                editorState.cursor_y--;
                editorState.cursor_x = editorState.rows[editorState.cursor_y].length;
            }
            break;
        case ARROW_RIGHT:
            if (row && editorState.cursor_x < row->length) {
                editorState.cursor_x++;
            } else if (row && editorState.cursor_x == row->length && !editorState.vim_emulation) {
                editorState.cursor_y++;
                editorState.cursor_x = 0;
            }
            break;
        case ARROW_UP:
            if (editorState.cursor_y != 0) {
                editorState.cursor_y--;
            }
            break;
        case ARROW_DOWN:
            if (editorState.cursor_y < editorState.numrows) {
                editorState.cursor_y++;
            }
            break;
    }

    row = (editorState.cursor_y >= editorState.numrows) ? NULL : &editorState.rows[editorState.cursor_y];
    int rowlen = row ? row->length : 0;

    if (editorState.cursor_x > rowlen) {
        editorState.cursor_x = rowlen;
    }
}

void setStatusMessage(int timeout, const char *format, ...) {
    va_list ap;
    va_start(ap, format);

    vsnprintf(editorState.statusmsg, sizeof(editorState.statusmsg), format, ap);

    va_end(ap);

    if (timeout >= 0) {
        editorState.statusmsg_time = time(NULL) + timeout;
    } else {
        editorState.statusmsg_time = time(NULL) + 36000;
    }
}
