#ifndef terminal_h
#define terminal_h

#define SCREENBUF_INIT {NULL, 0}

typedef unsigned int uint;

///Contains all characters that will be sent to the terminal
struct screen_buffer {
    ///The actual string buffer of the buffer
    char *str_buf;
    ///The length of the buffer
    int len;
};

void abAppend(struct screen_buffer *, const char *, int);
void abFree(struct screen_buffer *);
void refreshScreen();
void drawRows(struct screen_buffer *ab);
void scroll();
void drawStatusBar(struct screen_buffer *);
void drawMessageBar(struct screen_buffer *);
int getWindowSize(int *, int *);
int getCursorPosition(int *, int *);
void updateWindowSize();
uint countDigits(uint);


#endif /* terminal_h */
