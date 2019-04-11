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

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define UNUSED(x) (void)(x)

#define FEDIT_VERSION "0.0.1"
#define FEDIT_COMPILE_DATE __DATE__
#define FEDIT_TAB_STOP 8
#define FEDIT_QUIT_TIMES 2

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

#define VIM_INSERT_MODE (1<<0)
#define VIM_PROMPT_MODE (1<<1)
#define VIM_SEARCH_MODE (1<<2)
#define VIM_DELETE_MODE (1<<3)

//Editor row
typedef struct erow {
	int idx;
	int size;
	int rsize;
	char *chars;
	char *render;
	unsigned char *hl;
	int hl_open_comment;
} erow;


struct editorConfig {
	//Cursor position
	int cx, cy;
	int rx;
	int rowoff;
	int coloff;
	int screenrows;
	int screencols;
	int numrows;
	erow *row;
	int modified;
	int indentNewLine;
	int vimEmulation;
	char *filename;
	char statusmsg[80];
	time_t statusmsg_time;
	struct editorSyntax *syntax;
	struct termios orig_termios;
};

struct editorSyntax {
	char *filetype;
	char **filematch;
	char **keywords;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int flags;
};

//Screen buffer
struct abuf {
	char *str_buf;
	int len;
};

typedef struct textChange {
	int c;
	int changetype;
	int x, y;
} textChange;

typedef struct vimConfig {
	int mode;
	int delwordsSize;
	char *delwords;
	textChange *allchanges;
} vimConfig;

struct editorConfig E;
vimConfig VIM;

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

enum editorHighlight {
	HL_NORMAL = 0,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH,
	HL_COMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_KEYWORD3,
	HL_MLCOMMENT
};

enum editorChange {
	LINE_DELETE = 0,
	LINE_ADD,
	CHAR_DELETE,
	CHAR_ADD
};

char welcome[80];
char *openfile;

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
void handleSigWinch(int);

//Prototypes for terminal.c
void refreshScreen();
void drawRows();
void scroll();
void drawStatusBar(struct abuf *);
void drawMessageBar(struct abuf *);
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);
void updateWindowSize();

//Prototyped for appendbuffer.c
void abAppend(struct abuf *, const char *, int);
void abFree(struct abuf *);


//Prototypes for file.c
void file_open(char *);
int file_save();
char *rows_to_string(int *);

//Prototypes for rows.c
void insertRow(int, char *, size_t);
void updateRow(erow *row);
void rowInsertChar(erow *, int, int);
void rowDeleteChar(erow *, int);
void rowAppendString(erow *, char *, size_t);
int rowCxToRx(erow *, int);
int rowRxToCx(erow *, int);
void freeRow(erow *);
void deleteRow(int);

//Prototypes for editor.c
void deleteChar();
void insertChar(int);
void insertNewLine();
char *prompt(char *string, void (*callback)(char *, int));
void find();
void findCallback(char *, int);
void quit();

//Prototypes for highlight.c
void updateSyntax(erow *);
int syntaxToColor(int);
void selectSyntaxHighlight();
int isSeperator(int);

//Prototypes for commands.c
void parseCommandLine(char *);
int parseForceFlag(char *);
char *getCommand(char *, int);
char *getArgument(char *);

//Prototypes for delete.c
void addToDeleteWords(char);
void deleteNWords(int);

#endif