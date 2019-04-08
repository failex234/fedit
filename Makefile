fedit: src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c
	$(CC) src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c -o fedit -Wall -Wextra -pedantic
