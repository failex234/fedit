#ifndef FLX_FEDIT_H
#define FLX_FEDIT_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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

#define FEDIT_VERSION "0.0.1"
#define FEDIT_TAB_STOP 8
#define FEDIT_QUIT_TIMES 2

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

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

struct editorConfig E;

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
	HL_MLCOMMENT
};

char welcome[80];

//Prototypes for fedit.c
void init();
void moveCursor(int);
void setStatusMessage(const char *, ...);

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
void drawRows();
void scroll();
void drawStatusBar(struct abuf *);
void drawMessageBar(struct abuf *);
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);

//Prototypes for appendbuffer.c
void abAppend(struct abuf *, const char *, int);
void abFree(struct abuf *);

//Prototypes for file.c
void file_open(char *);
void file_save();
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

//Prototypes for highlight.c
void updateSyntax(erow *);
int syntaxToColor(int);
void selectSyntaxHighlight();
int isSeperator(int);

#endif