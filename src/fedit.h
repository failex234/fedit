#ifndef FLX_FEDIT_H
#define FLX_FEDIT_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#define FEDIT_VERSION "0.0.3"
#define FEDIT_COMPILE_DATE __TIMESTAMP__
#define FEDIT_TAB_STOP 8
#define FEDIT_QUIT_TIMES 2


void init();
void showHelp(const char *);
void showVersion(const char *);


#endif
