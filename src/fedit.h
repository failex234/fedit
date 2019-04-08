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

struct editorConfig {
	int screenrows;
	int screencols;
	struct termios orig_termios;
};

struct abuf {
	char *str_buf;
	int len;
};

struct editorConfig E;

//Prototypes for terminalmode.c
void enableRawMode();
void disableRawMode();

//Prototypes for errorhandling.c
void die(const char *);

//Prototypes for io.c
char readKey();
void processKeyPress();

//Prototypes for terminal.c
void clearScreen();
void drawRows();
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);

//Prototypes for appendbuffer.c
void abAppend(struct abuf *, const char *, int);
void abFree(struct abuf *);

#endif