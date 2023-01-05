#include "editor.h"
#include "signal.h"
#include "terminal.h"

void handle_SIGWINCH(int signal) {
    UNUSED(signal);

    updateWindowSize();

    if (editorState.cursor_y > editorState.screenrows) editorState.cursor_y = editorState.screenrows - 1;
    if (editorState.cursor_x > editorState.screencols) editorState.cursor_x = editorState.screencols - 1;

    refreshScreen();
}
