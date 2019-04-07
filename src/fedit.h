#ifndef FLX_FEDIT_H
#define FLX_FEDIT_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios;

//Prototypes for terminalmode.c
void enableRawMode();
void disableRawMode();

//Prototypes for errorhandling.c
void die(const char *);

#endif