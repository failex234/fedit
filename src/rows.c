#include "fedit.h"
void insertRow(int at, char *s, size_t len) {
    if (at < 0 || at > editorState.numrows) {
        return;
    }

    editorState.rows = realloc(editorState.rows, sizeof(erow) * (editorState.numrows + 1));
    memmove(&editorState.rows[at + 1], &editorState.rows[at], sizeof(erow) * (editorState.numrows - at));

    for (int i = at + 1; i <= editorState.numrows; i++) {
        editorState.rows[i].idx++;
    }

    editorState.rows[at].idx = at;

    //Set the length and allocate space for the string that should be appended
    editorState.rows[at].length = len;
    //malloc: memory corruption
    editorState.rows[at].chars = malloc(len + 1);

    //Copy the string to the correct location
    memcpy(editorState.rows[at].chars, s, len);

    //null terminate the string
    editorState.rows[at].chars[len] = '\0';

    editorState.rows[at].render_length = 0;
    editorState.rows[at].render = NULL;
    editorState.rows[at].highlight_types = NULL;
    editorState.rows[at].hl_open_comment = 0;
    updateRow(&editorState.rows[at]);

    editorState.numrows++;
    editorState.modified++;
}

//Replaces Tabs with 8 spaces to prevent "ghosting" of characters
//And also update the current rows
void updateRow(erow *row) {
    int tabs = 0;

    //Count the number of tabs
    for (int i = 0; i < row->length; i++) {
        if (row->chars[i] == '\t') {
            tabs++;
        }
    }

    free(row->render);
    row->render = malloc(row->length + tabs * (FEDIT_TAB_STOP - 1) + 1);

    //Replace tabs with 8 spaces
    int idx = 0;
    for (int i = 0; i < row->length; i++) {
        if (row->chars[i] == '\t') {
            row->render[idx++] = ' ';

            while (idx % FEDIT_TAB_STOP != 0) {
                row->render[idx++] = ' ';
            }
        } else {
            row->render[idx++] = row->chars[i];
        }
    }

    //null terminate string
    row->render[idx] = '\0';

    //set the render length
    row->render_length = idx;

    updateSyntax(row);
}

int rowCxToRx(erow *row, int cx) {
    int rx = 0;

    for (int i = 0; i < cx; i++) {
        if (row->chars[i] == '\t') {
            rx += (FEDIT_TAB_STOP - 1) - (rx % FEDIT_TAB_STOP);
        }
        rx++;
    }

    if (!editorState.disable_linenums) {
        rx += countDigits(editorState.numrows) + 1;
    }
    return rx;
}

int rowRxToCx(erow *row, int rx) {
    int cur_rx = 0;

    int cx;
    for (cx = 0; cx < row->length; cx++) {
        if (row->chars[cx] == '\t') {
            cur_rx += (FEDIT_TAB_STOP - 1) - (cur_rx % FEDIT_TAB_STOP);
        }
        cur_rx++;

        if (cur_rx > rx) {
            return cx;
        }
    }

    return cx;
}

void rowInsertChar(erow *row, int at, int c) {
    if (at < 0 || at > row->length) {
        at = row->length;
    }

    //Reallocate space for the new character
    row->chars = realloc(row->chars, row->length + 2);
    memmove(&row->chars[at + 1], &row->chars[at], row->length - at + 1);

    //Increase the length by 1 and then insert the character
    row->length++;
    row->chars[at] = c;

    updateRow(row);
    editorState.modified++;
}

void rowDeleteChar(erow *row, int at) {
    if (at < 0 || at >= row->length) {
        return;
    }

    //Reallocate space for the deleted character
    memmove(&row->chars[at], &row->chars[at + 1], row->length - at);
    //Decrease length by 1
    row->length--;

    updateRow(row);
    editorState.modified++;
}

void rowAppendString(erow *row, char *string, size_t len) {
    //Reallocate space for the current rows + the new string
    row->chars = realloc(row->chars, row->length + len + 1);
    memcpy(&row->chars[row->length], string, len);

    //Add the length of the string to the rows length
    row->length += len;
    //Add the null-terminator
    row->chars[row->length] = '\0';

    updateRow(row);
    editorState.modified++;
}

void freeRow(erow *row) {
    free(row->render);
    free(row->chars);
    free(row->highlight_types);
}

void deleteRow(int at) {
    if (at < 0 || at >= editorState.numrows) {
        return;
    }

    //Free the space of the rows
    freeRow(&editorState.rows[at]);
    memmove(&editorState.rows[at], &editorState.rows[at + 1], sizeof(erow) * (editorState.numrows - at - 1));

    for (int i = at; i < editorState.numrows - 1; i++) {
        editorState.rows[i].idx--;
    }

    editorState.numrows--;
    editorState.modified++;
}
