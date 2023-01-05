#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "editor.h"
#include "error.h"
#include "fedit.h"
#include "file.h"
#include "git.h"
#include "rows.h"
#include "signal.h"
#include "terminal.h"
#include "terminal_io.h"

int main(int argc, char **argv) {
    int enableVim = 0;
    cmdline_openfile = NULL;

    enableVim = alwaysVim;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            showHelp(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
            showVersion(argv[0]);
            return 0;
        } else if (!strcmp(argv[i], "--vim") || !strcmp(argv[i], "-e")) {
            enableVim = 1;
        } else {
            //Check if there's an argument
            char *loc = strstr(argv[i], "-");
            if (loc && argv[i][0] == '-')  {
                printf("unknown argument %s\n", argv[i]);

                return 1;
            } else {
                cmdline_openfile = argv[i];
                break;
            }
        }
    }
    enableRawMode();
    init();

    if (enableVim) {
        editorState.vim_emulation = 1;
    } else {
        setStatusMessage(0, "HELP: Ctrl+S = save | Ctrl+Q = quit | Ctrl+F = find | Ctrl+D = delete | Ctrl+L = del line");
    }

    if (cmdline_openfile) {
        file_open(cmdline_openfile);
    }

    while (1) {
        refreshScreen();
        processKeyPress();
    }
    return 0;
}

void init() {
    //Set initial values
    editorState.cursor_x = 0;
    editorState.cursor_y = 0;
    editorState.cursor_rendered_x = 0;
    editorState.rowoff = 0;
    editorState.coloff = 0;
    editorState.numrows = 0;
    editorState.disable_highlight = 0;
    editorState.disable_linenums = 0;
    editorState.rows = NULL;
    editorState.modified = 0;
    editorState.filepath = NULL;
    editorState.filename = NULL;
    editorState.statusmsg[0] = '\0';
    editorState.statusmsg_time = 0;
    editorState.syntax = NULL;
    editorState.indent_newline = 0;
    editorState.vim_emulation = 0;

    vimState.mode = 0;
    vimState.numinput_buffer_len = 0;
    vimState.numinput_buffer = NULL;

    if (getWindowSize(&editorState.screenrows, &editorState.screencols) == -1) {
        die("getWindowSize");
    }

    signal(SIGWINCH, handle_SIGWINCH);

    editorState.screenrows -= 2;
}

void showHelp(const char *prgname) {
    printf("usage: %s [arguments] files...\n", prgname);
    printf("\narguments:\n");
    printf("--vim 	 | -e\t\t\t- enable vim emulation mode\n");
    printf("--help	 | -h\t\t\t- show help menu\n");
    printf("--version| -v\t\t\t- show version information\n");
}

void showVersion(const char *prgname) {
#ifdef ENABLE_DEV
    char *commit_hash = (char*) malloc(sizeof(char)*7);
    memcpy(commit_hash, git_CommitSHA1(), 6);
    commit_hash[6] = '\0';
    printf("%s development version (compiled on %s)\nCommit Hash: %s%s\n", prgname, FEDIT_COMPILE_DATE, commit_hash, git_AnyUncommittedChanges() ? " + uncommitted changes" : "");
#else
    printf("%s version %s (compiled on %s)\n", prgname, FEDIT_VERSION, FEDIT_COMPILE_DATE);
#endif
}
