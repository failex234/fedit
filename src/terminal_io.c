#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "commands.h"
#include "editor.h"
#include "fedit.h"
#include "file.h"
#include "error.h"
#include "rows.h"
#include "terminal_io.h"
#include "vim_delete.h"


int readKey() {
    int nread;
    char c;

    //Read key from stdin
    while ((nread = read(STDOUT_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }

    //Check for control characters
    if (c == '\x1b') {
        char seq[3];

        //Make sure, that the input is a escape sequence
        if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) {
            return '\x1b';
        }

        //Determine control character
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) {
                    return '\x1b';
                }

                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1':
                            return HOME_KEY;
                        case '3':
                            return DEL_KEY;
                        case '4':
                            return END_KEY;
                        case '5':
                            return PAGE_UP;
                        case '6':
                            return PAGE_DOWN;
                        case '7':
                            return HOME_KEY;
                        case '8':
                            return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A':
                        return ARROW_UP;
                    case 'B':
                        return ARROW_DOWN;
                    case 'C':
                        return ARROW_RIGHT;
                    case 'D':
                        return ARROW_LEFT;
                    case 'H':
                        return HOME_KEY;
                    case 'F':
                        return END_KEY;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H':
                    return HOME_KEY;
                case 'F':
                    return END_KEY;
            }
        }
        return '\x1b';
    } else if (editorState.vim_emulation && !(vimState.mode & VIM_INSERT_MODE) && !(vimState.mode & VIM_PROMPT_MODE) &&
               (c == 'h' || c == 'j' || c == 'k' || c == 'l')) {
        switch (c) {
            case 'h':
                return ARROW_LEFT;
            case 'j':
                return ARROW_DOWN;
            case 'k':
                return ARROW_UP;
            case 'l':
                return ARROW_RIGHT;
        }
    }
    return c;
}

//Does stuff when certain keys are pressed
void processKeyPress() {
    static int quit_times = FEDIT_QUIT_TIMES;

    int c = readKey();

    if (!editorState.vim_emulation) {
        switch (c) {
            case '\r':
                insertNewLine();
                break;
            case CTRL_KEY('q'):
                if (editorState.modified && quit_times > 0) {
                    setStatusMessage(0, "Warning! Your file has unsaved changes! Press Ctrl+Q %d more time%s to quit.",
                                     quit_times, quit_times > 1 ? "s" : "");
                    quit_times--;
                    return;
                }
                quit(0);
                break;
            case CTRL_KEY('s'):
                file_save();
                break;
            case CTRL_KEY('f'):
                find();
                break;
            case HOME_KEY:
                editorState.cursor_x = 0;
                break;
            case END_KEY:
                //Place the cursor at the end of the line
                if (editorState.cursor_y < editorState.numrows) {
                    editorState.cursor_x = editorState.rows[editorState.cursor_y].length;
                }
                break;
            case BACKSPACE:
            case CTRL_KEY('h'):
            case DEL_KEY:
                if (c == DEL_KEY) {
                    moveCursor(ARROW_RIGHT);
                }
                deleteChar();
                break;
            case CTRL_KEY('d'): {
                char *str_num = prompt("Delete next %s words", NULL);

                if (str_num) {
                    int num = atoi(str_num);

                    if (num > 0) {
                        deleteNWords(num);
                    }
                    setStatusMessage(0, "");
                    free(str_num);
                }
                break;
            }
            case CTRL_KEY('l'):
                deleteRow(editorState.cursor_y);
                break;
            case PAGE_UP:
            case PAGE_DOWN: {
                if (c == PAGE_UP) {
                    editorState.cursor_y = editorState.rowoff;

                    int times = editorState.screenrows;
                    while (times--) {
                        moveCursor(ARROW_UP);
                    }
                } else if (c == PAGE_DOWN) {
                    editorState.cursor_y = editorState.rowoff + editorState.screenrows - 1;

                    if (editorState.cursor_y > editorState.numrows) {
                        editorState.cursor_y = editorState.numrows;
                    }

                    int times = editorState.screenrows;
                    while (times--) {
                        moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                    }
                }
                break;
            }
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
                moveCursor(c);
                break;
            case '\x1b':
                break;
            default:
                insertChar(c);
                break;
        }
        quit_times = FEDIT_QUIT_TIMES;
    } else if (!(vimState.mode & VIM_DELETE_MODE)) {
        switch (c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    addToDeleteWords(c);
                    setStatusMessage(0, "%s", vimState.numinput_buffer);
                }
                break;
            case 'G':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    if (vimState.numinput_buffer) {
                        int num = atoi(vimState.numinput_buffer);
                        if (num > editorState.numrows) num = editorState.numrows;
                        if (num <= 0) num = 1;

                        goToLine(num);
                        free(vimState.numinput_buffer);
                        vimState.numinput_buffer_len = 0;
                        vimState.numinput_buffer = NULL;
                        setStatusMessage(0, "");
                    } else {
                        goToLine(1);
                    }
                }
                break;
            case 'g':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    if (vimState.mode & VIM_GOTO_MODE) {
                        goToLine(editorState.numrows);
                        vimState.mode = 0;
                    } else {
                        vimState.mode = VIM_GOTO_MODE;
                    }
                }
                break;
            case 'i':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    setStatusMessage(-1, "--- INSERT ---");
                    vimState.mode = VIM_INSERT_MODE;
                }
                break;
            case 'o':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    if (editorState.cursor_y < editorState.numrows) {
                        editorState.cursor_x = editorState.rows[editorState.cursor_y].length;
                    }

                    insertNewLine();

                    setStatusMessage(-1, "--- INSERT ---");
                    vimState.mode = VIM_INSERT_MODE;
                }
                break;
            case 'd':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    vimState.mode |= VIM_DELETE_MODE;
                    setStatusMessage(-1, "--- DELETE ---");
                }
                break;
            case '\x1b':
                vimState.mode = 0;
                setStatusMessage(0, "");

                if (vimState.numinput_buffer_len > 0) {
                    free(vimState.numinput_buffer);
                    vimState.numinput_buffer_len = 0;
                    vimState.numinput_buffer = NULL;
                }
                break;
            case ':':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    char *cmd = prompt(":%s", NULL);

                    if (cmd) {
                        parse_command_prompt(cmd);
                    } else {
                        setStatusMessage(0, "");
                    }
                }
                break;
            case '/':
                if (vimState.mode == 0) {
                    vimState.mode = VIM_SEARCH_MODE;
                    find();
                } else if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                }
                break;
            case BACKSPACE:
            case CTRL_KEY('h'):
            case DEL_KEY:
                if (vimState.mode & VIM_INSERT_MODE || c == DEL_KEY) {
                    if (c == DEL_KEY) {
                        moveCursor(ARROW_RIGHT);
                    }
                    deleteChar();
                }
                break;
            case HOME_KEY:
                editorState.cursor_x = 0;
                break;
            case END_KEY:
                //Place the cursor at the end of the line
                if (editorState.cursor_y < editorState.numrows) {
                    editorState.cursor_x = editorState.rows[editorState.cursor_y].length;
                }
                break;
            case '\r':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertNewLine();
                }
                break;
            case '\t':
                insertChar(c);
                break;
            case ARROW_UP:
            case ARROW_DOWN:
            case ARROW_LEFT:
            case ARROW_RIGHT:
                moveCursor(c);
                break;
            case PAGE_UP:
            case PAGE_DOWN: {
                if (c == PAGE_UP) {
                    editorState.cursor_y = editorState.rowoff;

                    int times = editorState.screenrows;
                    while (times--) {
                        moveCursor(ARROW_UP);
                    }
                } else if (c == PAGE_DOWN) {
                    editorState.cursor_y = editorState.rowoff + editorState.screenrows - 1;

                    if (editorState.cursor_y > editorState.numrows) {
                        editorState.cursor_y = editorState.numrows;
                    }

                    int times = editorState.screenrows;
                    while (times--) {
                        moveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
                    }
                }
                break;
            }
            case 'x':
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                } else {
                    moveCursor(ARROW_RIGHT);
                    deleteChar();
                }
                break;
            default:
                if (iscntrl(c)) break;
                if (vimState.mode & VIM_INSERT_MODE) {
                    insertChar(c);
                }
                break;
        }
    } else if (vimState.mode & VIM_DELETE_MODE) {
        switch (c) {
            case '\x1b':
                vimState.mode = 0;
                setStatusMessage(0, "");

                if (vimState.numinput_buffer) {
                    free(vimState.numinput_buffer);
                    vimState.numinput_buffer_len = 0;
                }
                break;
            case 'd':
                if (vimState.numinput_buffer) {
                    deleteNWords(atoi(vimState.numinput_buffer));
                    free(vimState.numinput_buffer);
                    vimState.numinput_buffer_len = 0;
                    vimState.numinput_buffer = NULL;
                } else {
                    deleteRow(editorState.cursor_y);
                }
                vimState.mode = 0;
                setStatusMessage(0, "");
                break;
            case 'e':
                if (vimState.numinput_buffer) {
                    deleteNWords(atoi(vimState.numinput_buffer));
                    free(vimState.numinput_buffer);
                    vimState.numinput_buffer_len = 0;
                    vimState.numinput_buffer = NULL;
                } else {
                    deleteNWords(1);
                }
                vimState.mode = 0;
                setStatusMessage(0, "");
                break;
            default:
                if (!isdigit(c)) break;
                addToDeleteWords(c);
                break;

        }
    }


}

//Enablke the terminal raw mode so that keystrokes directly get send to the program
void enableRawMode() {

    //Get the current teminal mode and attributes and save them in the struct orig_termios
    //Also tell the library to call disableRawMode on exit (atexit)
    if (tcgetattr(0, &editorState.orig_termios) == -1) {
        die("tcsetattr");
    }
    atexit(disableRawMode);

    //Create a new temporary new struct to set the new flags
    struct termios raw = editorState.orig_termios;

    //Disable Ctrl+Q (XON) and Ctrl+S (XOFF) (Both are used for software control flow)
    //They are unneeded for today's computers
    //Also disable Ctrl+M (ICRNL)
    //There is another way to send SIGINT to the program. We disable this with (BRKINT)
    //Also we disable some kind of parity check? This is not used in modern terminals anymore (INPCK)
    //(ISTRIP) causes the 8th bit of each input byte to be stripped, meaning it will set it to 0. This is probably already turned off.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    //Disable the terminal translation from \n to \r\n
    raw.c_oflag &= ~(OPOST);

    //Set the character length to 8 bits per byte (CS8)
    raw.c_cflag |= (CS8);

    //Disable echo flag (So that the typed character will NOT be echoed back) (ECHO)
    //Disable canonical mode (The character now directly gets send to the program) (ICANON)
    //Disable Ctrl+V (IEXTEN)
    //And directly send signals like SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z) directly to the process (ISIG)
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    //Both important for the wanted behaviour of the read function / call
    //The index VMIN defines how many bytes the read call has to read before it returns anything
    //We set it to 0 so that read() returns as soon as there is any input to be read
    //Otherwise we're unable to execute code while we're waiting for user input
    raw.c_cc[VMIN] = 0;

    //The index VTIME defines how long read() should wait before returning a value
    //The value sets the delay in 1/10 seconds. So we set the delay to 1/10 second
    raw.c_cc[VTIME] = 1;

    //Set the new terminal mode
    if (tcsetattr(0, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

//Restore the "original" terminal mode
void disableRawMode() {
    //try restore the original mode
    if (tcsetattr(0, TCSAFLUSH, &editorState.orig_termios) == -1) {
        die("tcsetattr");
    }
}

void clear_screen() {
    //Erase screen
    write(STDOUT_FILENO, "\x1b[2J", 4);
    //Place cursor to default (1,1) position
    write(STDOUT_FILENO, "\x1b[H", 3);
}

