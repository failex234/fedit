fedit: src/fedit.h src/highlight.h src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c src/editor.c src/highlight.c src/commands.c src/delete.c
	$(CC) src/fedit.c src/terminalmode.c src/errorhandling.c src/appendbuffer.c src/io.c src/terminal.c src/rows.c src/file.c src/editor.c src/highlight.c src/variables.c src/commands.c src/delete.c -o fedit -Wall -Wextra -pedantic

install: fedit
	cp fedit ${DESTDIR}/usr/bin

uninstall:
	rm -f ${DESTDIR}/usr/bin/fedit
