#include "fedit.h"

//Enablke the terminal raw mode so that keystrokes directly get send to the program
void enableRawMode() {

    //Get the current teminal mode and attributes and save them in the struct orig_termios
    //Also tell the library to call disableRawMode on exit (atexit)
    if (tcgetattr(0, &orig_termios) == -1) {
        die("tcsetattr");
    }
    atexit(disableRawMode);

    //Create a new temporary new struct to set the new flags
    struct termios raw = orig_termios;

    //Disable Ctrl+Q (XON) and Ctrl+S (XOFF) (Both are used for software control flow)
    //They are unneeded for today's computers
    //Also disable Ctrl+M (ICRNL)
    //There is another way to send SIGINT to the program. We disable this with (BRKINT)
    //Also we disable some kind of parity check? This is not used in modern terminals anymore (INPCK)
    //(ISTRIP) causes the 8th bit of each input byte to be stripped, meaning it will set it to 0. This is probably already turned off.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    //Disable the terminal translation from \n to \r\n
    raw.c_oflag &= ~(OPOST);

    //Set the character size to 8 bits per byte (CS8)
    raw.c_cflag |= (CS8);

    //Disable echo flag (So that the typed character will NOT be echoed back) (ECHO)
    //Disable canonical mode (The character now directly gets send to the program) (ICANON)
    //Disable Ctrl+V (IEXTEN)
    //And directly send signals like SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z) directly to the process (ISIG)
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    //Both important for the wanted behaviour of the read function / call
    //The index VMIN defines how many bytes the read call has to read before it returns anything
    //We set it to 0 so that read() returns as soon as there is any input to be read
    //Otherwise we're unable to execute code while we're waiting for user input
    raw.c_cc[VMIN] = 0;

    //The index VTIME defines how long read() should wait before returning a value
    //The value sets the delay in 1/10 seconds. So we set the delay to 1/10 second
    raw.c_cc[VTIME] = 1;

    //Set the new terminal mode
    if (tcsetattr(0, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

//Restore the "original" terminal mode
void disableRawMode() {
    //try restore the original mode
    if (tcsetattr(0, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}