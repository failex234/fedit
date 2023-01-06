#ifndef editor_h
#define editor_h

#include <termios.h>
#include <time.h>

///The current editors state
struct editorState {
    ///Cursor x and y position in the file
    int cursor_x, cursor_y;
    ///Cursor rendered x position (Influenced by tabs and enabled line numbers)
    int cursor_rendered_x;
    ///The current rows offset on the screen (The rows the cursor is in)
    int rowoff;
    ///The current columns offset on the screen (The column the cursor is in)
    int coloff;
    ///Number of rows on screen
    int screenrows;
    ///Number of columns (Terminal length)
    int screencols;
    ///Number of rows (Terminal length)
    int numrows;
    ///Disable syntax highlighting
    int disable_highlight;
    ///Disable line numbering
    int disable_linenums;
    ///Text rows representing the contents of the opened file
    struct erow *rows;
    ///Whether the file was modified
    int modified;
    ///Enable the vim emulation mode
    int vim_emulation;
    ///The path to the opened file as given by the user
    char *filepath;
    ///The discrete file name
    char *filename;
    ///Contents of the status message bar
    char statusmsg[80];
    ///The message bar display timeout
    time_t statusmsg_time;
    ///The syntax highlighting currently in use
    struct editorSyntax *syntax;
    ///The original termios before the editor was drawn (terminal mode info)
    struct termios orig_termios;
};

///Additional state when in vim mode
struct vimState {
    ///The current vim mode. enum `vimModes` contains all modes
    int mode;
    ///Buffer for number input before entering delete or goto mode
    ///When user wants to delete next n words or goto line n, this buffer contains this number as a string
    char *numinput_buffer;
    ///The length of the numinput buffer
    int numinput_buffer_len;
};

///Current mutable editorState
struct editorState editorState;
///Current mutable vimState
struct vimState vimState;

///Vim operation modes
enum vimModes {
    VIM_INSERT_MODE = 1,
    VIM_PROMPT_MODE = 2,
    VIM_SEARCH_MODE = 4,
    VIM_DELETE_MODE = 8,
    VIM_GOTO_MODE = 16
};

///Welcome message buffer
char welcome[80];

void deleteChar();
void insertChar(int);
void insertNewLine();
char *prompt(char *string, void (*callback)(char *, int));
void find();
void findCallback(char *, int);
void moveCursor(int);
void setStatusMessage(int, const char *, ...);
void quit(int status);
void goToLine(int);
void editor_initialize_data();


#endif /* editor_h */
