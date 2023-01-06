#ifndef terminal_io_h
#define terminal_io_h

#define CTRL_KEY(k) ((k) & 0x1f)

///Special keys used in the editor
enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT     = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN,

};

int readKey();
void processKeyPress();
void enableRawMode();
void disableRawMode();
void clear_screen();

#endif /* terminal_io_h */
