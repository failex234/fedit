#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "editor.h"
#include "rows.h"

void addToDeleteWords(char numAsChar) {
    if (!isdigit(numAsChar)) return;
    vimState.numinput_buffer = realloc(vimState.numinput_buffer, ++vimState.numinput_buffer_len + 1);
    vimState.numinput_buffer[vimState.numinput_buffer_len - 1] = numAsChar;
    vimState.numinput_buffer[vimState.numinput_buffer_len] = '\0';
}


void deleteNWords(int number) {
    int spaces = 0;
    int lastchar = 0;
    int startidx = 0;
    int endidx = 0;
    int i = 0;

    if (editorState.cursor_y > editorState.numrows || editorState.numrows == 0) {
        return;
    }

    char *currRow = editorState.rows[editorState.cursor_y].chars;
    int rowSize = editorState.rows[editorState.cursor_y].length;

    //Find the last space before the word
    for (i = 0; i < editorState.cursor_x; i++) {
        if (currRow[i] == ' ') {
            startidx = i;
        }
    }

    if (editorState.cursor_x) {
        i = ++startidx;
    } else {
        startidx = 0;
        i = startidx;
    }

    while(spaces != number && i < rowSize) {
        if (currRow[i] == ' ' || currRow[i] == '\"') {
            lastchar = currRow[i];
            spaces++;
        }
        i++;
    }

    endidx = i - (lastchar == '\"' ? 2 : 1);

    char *newrow = malloc(startidx + (rowSize - endidx));
    memcpy(newrow, currRow, startidx);
    memcpy(&newrow[startidx], &currRow[endidx + 1], rowSize - endidx - 1);

    newrow[startidx + rowSize - endidx - 1] = '\0';

    deleteRow(editorState.cursor_y);
    insertRow(editorState.cursor_y, newrow, startidx + rowSize - endidx - 1);

    if (editorState.cursor_x > editorState.rows[editorState.cursor_y].length) editorState.cursor_x = editorState.rows[editorState.cursor_y].length - 1;

}
