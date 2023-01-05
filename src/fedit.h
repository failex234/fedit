#ifndef FLX_FEDIT_H
#define FLX_FEDIT_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "signalhandler.h"

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define UNUSED(x) (void)(x)

#define FEDIT_VERSION "0.0.3"
#define FEDIT_COMPILE_DATE __TIMESTAMP__
#define FEDIT_TAB_STOP 8
#define FEDIT_QUIT_TIMES 2

///Abstract representation of a rows in the editor
struct erow {
    ///Rows index in the whole file
    int idx;
    ///Character length of the row
    int length;
    ///Length of the rendered contents meant to be displayed
    int render_length;
    ///Raw sequence of the line contents
    char *chars;
    ///Raw sequence of the rendered contents
    char *render;
    ///Syntax highlighting type per character
    unsigned char *highlight_types;
    ///This line is part of a comment
    int hl_open_comment;
};

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
    ///Indent the next new line with n-tabs
    int indent_newline;
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

///Syntax highlighting
struct editorSyntax {
    ///Syntax name as displayed in the editor
    char *pretty_name;
    ///The shortform of the syntax when used for setting the syntax manually
    char *syntaxname;
    ///All file extensions this syntax will be used for by default (including period prefix)
    char **filematch;
    ///Contains all the keywords of the syntax that will be colored (suffixes "|" and "<" change the color)
    char **keywords;
    ///Single line comment initiator string eg. //
    char *singleline_comment_start;
    ///Multi line comment initiator string eg. /*
    char *multiline_comment_start;
    ///Multi line comment end string eg. */
    char *multiline_comment_end;
    ///Features / Additional options for the syntax highlighting. Found in enum `highlightFlags`
    int flags;
};

///Contains all characters that will be sent to the terminal
struct screen_buffer {
    ///The actual string buffer of the buffer
    char *str_buf;
    ///The length of the buffer
    int len;
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

///Special keys used in the editor
enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT 	= 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN,

};

///Categories of syntax highlighting
enum editorHighlight {
    HL_NORMAL = 0,
    HL_STRING,
    HL_NUMBER,
    HL_MATCH,
    HL_COMMENT,
    HL_KEYWORD1,
    HL_KEYWORD2,
    HL_KEYWORD3,
    HL_MLCOMMENT,
    HL_TEMPLATE_FORMAT,
};

///Features of syntax highlighting
enum highlightFlags {
    HL_HIGHLIGHT_NUMBERS = 1,
    HL_HIGHLIGHT_STRINGS = 2,
    HL_HIGHLIGHT_LTGT = 4,
    HL_HIGHLIGHT_TEMPLATES = 8,
    HL_HIGHLIGHT_PERCENT_FORMAT_SP = 16
};

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
///File name of the file to be read. When given via cmdline argument
char *cmdline_openfile;

//Prototypes for fedit.c
void init();
void moveCursor(int);
void setStatusMessage(int, const char *, ...);
void showHelp(const char *);
void showVersion(const char *);

//Prototypes for terminalmode.c
void enableRawMode();
void disableRawMode();

//Prototypes for errorhandling.c
void die(const char *);

//Prototypes for io.c
int readKey();
void processKeyPress();

//Prototypes for terminal.c
void refreshScreen();
void drawRows(struct screen_buffer *ab);
void scroll();
void drawStatusBar(struct screen_buffer *);
void drawMessageBar(struct screen_buffer *);
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);
void updateWindowSize();
uint countDigits(uint);

//Prototyped for appendbuffer.c
void abAppend(struct screen_buffer *, const char *, int);
void abFree(struct screen_buffer *);


//Prototypes for file.c
void file_open(char *);
int file_save();
char *rows_to_string(int *);
void set_file(const char *);

//Prototypes for rows.c
void insertRow(int, char *, size_t);
void updateRow(struct erow *row);
void rowInsertChar(struct erow *, int, int);
void rowDeleteChar(struct erow *, int);
void rowAppendString(struct erow *, char *, size_t);
int rowCxToRx(struct erow *, int);
int rowRxToCx(struct erow *, int);
void freeRow(struct erow *);
void deleteRow(int);

//Prototypes for editor.c
void deleteChar();
void insertChar(int);
void insertNewLine();
char *prompt(char *string, void (*callback)(char *, int));
void find();
void findCallback(char *, int);
void quit();
void goToLine(int);

//Prototypes for highlight.c
void updateSyntax(struct erow *);
int syntaxToColor(int);
void determineSyntaxHighlight();
void forceSyntaxHighlighting(int, struct editorSyntax*);
int is_seperator(int);
int is_format_seperator(int);
struct editorSyntax *findSyntax(const char *);

//Prototypes for commands.c
void parseCommandLine(char *);
int parseForceFlag(char *);
char *getCommand(char *, int);
char *getArgument(char *);
int countChars(const char *, int, char);

//Prototypes for delete.c
void addToDeleteWords(char);
void deleteNWords(int);

#endif
