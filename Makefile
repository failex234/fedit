fedit: src/fedit.h src/highlight.h src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c src/editor.c src/highlight.c src/commands.c
	$(CC) -g src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c src/editor.c src/highlight.c src/commands.c -o fedit -Wall -Wextra -pedantic
