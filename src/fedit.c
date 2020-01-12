#include "fedit.h"
#include "config.h"
#include "variables.h"

int main(int argc, char **argv)
{
	int enableVim = 0;
	openfile = NULL;
	vars = NULL;
	vars_len = 0;

	enableVim = alwaysVim;

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
		{
			showHelp(argv[0]);
			return 0;
		}
		else if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v"))
		{
			showVersion(argv[0]);
			return 0;
		}
		else if (!strcmp(argv[i], "--vim") || !strcmp(argv[i], "-e"))
		{
			enableVim = 1;
		}
		else
		{
			//Check if there's an argument
			char *loc = strstr(argv[i], "-");
			if (loc && argv[i][0] == '-')
			{
				printf("unknown argument %s\n", argv[i]);

				return 1;
			}
			else
			{
				openfile = argv[i];
				break;
			}
		}
	}
	enableRawMode();
	init();

	if (enableVim)
	{
		E.vimEmulation = 1;
	}
	else
	{
		setStatusMessage(0, "HELP: Ctrl+S = save | Ctrl+Q = quit | Ctrl+F = find | Ctrl+D = delete | Ctrl+L = del line");
	}

	if (openfile)
	{
		file_open(openfile);
	}

	while (1)
	{
		refreshScreen();
		processKeyPress();
	}
	return 0;
}

void init()
{
	//Set initial values
	E.cx = 0;
	E.cy = 0;
	E.rx = 0;
	E.rowoff = 0;
	E.coloff = 0;
	E.numrows = 0;
	E.row = NULL;
	E.modified = 0;
	E.filename = NULL;
	E.statusmsg[0] = '\0';
	E.statusmsg_time = 0;
	E.syntax = NULL;
	E.indentNewLine = 0;
	E.vimEmulation = 0;

	VIM.mode = 0;
	VIM.delwordsSize = 0;
	VIM.delwords = NULL;
	VIM.allchanges = NULL;

	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
	{
		die("getWindowSize");
	}

	signal(SIGWINCH, handleSigWinch);

	E.screenrows -= 2;
}

void moveCursor(int key)
{
	erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	switch (key)
	{
	case ARROW_LEFT:
		if (E.cx != 0)
		{
			E.cx--;
		}
		else if (E.cy > 0 && !E.vimEmulation)
		{
			E.cy--;
			E.cx = E.row[E.cy].size;
		}
		break;
	case ARROW_RIGHT:
		if (row && E.cx < row->size)
		{
			E.cx++;
		}
		else if (row && E.cx == row->size && !E.vimEmulation)
		{
			E.cy++;
			E.cx = 0;
		}
		break;
	case ARROW_UP:
		if (E.cy != 0)
		{
			E.cy--;
		}
		break;
	case ARROW_DOWN:
		if (E.cy < E.numrows)
		{
			E.cy++;
		}
		break;
	}

	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;

	if (E.cx > rowlen)
	{
		E.cx = rowlen;
	}
}

void setStatusMessage(int timeout, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	vsnprintf(E.statusmsg, sizeof(E.statusmsg), format, ap);

	va_end(ap);

	if (timeout >= 0)
	{
		E.statusmsg_time = time(NULL) + timeout;
	}
	else
	{
		E.statusmsg_time = time(NULL) + 36000;
	}
}

void showHelp(const char *prgname)
{
	printf("usage: %s [arguments] files...\n", prgname);
	printf("\narguments:\n");
	printf("--vim 	 | -e\t\t\t- enable vim emulation mode\n");
	printf("--help	 | -h\t\t\t- show help menu\n");
	printf("--version| -v\t\t\t- show version information\n");
}

void showVersion(const char *prgname)
{
	printf("%s version %s (%s)\n", prgname, FEDIT_VERSION, FEDIT_COMPILE_DATE);
}
