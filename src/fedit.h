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

//Editor row
typedef struct erow {
	int size;
	int rsize;
	char *chars;
	char *render;
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
	struct termios orig_termios;
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
void file_open(char *filename);
void file_save();
char *rows_to_string(int *);

//Prototypes for rows.c
void appendRow(char *, size_t);
void updateRow(erow *row);
int rowCxToRx(erow *row, int cx);

//Prototypes for editor.c
void rowInsertChar(erow *, int, int);
void rowDeleteChar(erow *, int);
void deleteChar();
void insertChar(int);

#endif