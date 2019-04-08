fedit: src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c
	$(CC) src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c -o fedit -Wall -Wextra -pedantic
