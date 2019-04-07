#include "fedit.h"

int main() {
	enableRawMode();

	//Read until there are no more bytes OR the pressed character is q
	while (1) {
		char c = '\0';
		if (read(0, &c, 1) == -1 && errno != EAGAIN) {
			die("read");
		}
		//Don't print the character if the "character" is a control character
		//Printing a carriage return and a newline since we disabled the
		//automatic translation of \n to \r\n by the terminal
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == CTRL_KEY('q')) break;
	}
	return 0;
}
