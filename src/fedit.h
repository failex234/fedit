#ifndef FLX_FEDIT_H
#define FLX_FEDIT_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}

#define FEDIT_VERSION "0.0.1"

struct editorConfig {
	//Cursor position
	int cx, cy;
	int screenrows;
	int screencols;
	struct termios orig_termios;
};

struct abuf {
	char *str_buf;
	int len;
};

struct editorConfig E;

enum editorKey {
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
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);

//Prototypes for appendbuffer.c
void abAppend(struct abuf *, const char *, int);
void abFree(struct abuf *);

#endif