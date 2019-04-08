#include "fedit.h"
void insertRow(int at, char *s, size_t len) {
	if (at < 0 || at > E.numrows) {
		return;
	}

	E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
	memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));

	for (int i = at + 1; i <= E.numrows; i++) {
		E.row[i].idx++;
	}

	E.row[at].idx = at;
	
	//Set the length and allocate space for the string that should be appended
	E.row[at].size = len;
	E.row[at].chars = malloc(len + 1);
	
	//Copy the string to the correct location
	memcpy(E.row[at].chars, s, len);
	
	//null terminate the string
	E.row[at].chars[len] = '\0';
	
	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	E.row[at].hl = NULL;
	E.row[at].hl_open_comment = 0;
	updateRow(&E.row[at]);
	
	E.numrows++;
	E.modified++;
}

//Replaces Tabs with 8 spaces to prevent "ghosting" of characters
//And also update the current row
void updateRow(erow *row) {
	int tabs = 0;
	
	//Count the number of tabs
	for (int i = 0; i < row->size; i++) {
		if (row->chars[i] == '\t') {
			tabs++;
		}
	}
	
	free(row->render);
	row->render = malloc(row->size + tabs * (FEDIT_TAB_STOP - 1) + 1);
	
	//Replace tabs with 8 spaces
	int idx = 0;
	for (int i = 0; i < row->size; i++) {
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
	
	//set the render size
	row->rsize = idx;

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
	return rx;
}

int rowRxToCx(erow *row, int rx) {
	int cur_rx = 0;

	int cx;
	for (cx = 0; cx < row->size; cx++) {
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
	if (at < 0 || at > row->size) {
		at = row->size;
	}
	
	//Reallocate space for the new character
	row->chars = realloc(row->chars, row->size + 2);
	memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	
	//Increase the size by 1 and then insert the character
	row->size++;
	row->chars[at] = c;
	
	updateRow(row);
	E.modified++;
}

void rowDeleteChar(erow *row, int at) {
	if (at < 0 || at >= row->size) {
		return;
	}

	//Reallocate space for the deleted character
	memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
	//Decrease size by 1
	row->size--;

	updateRow(row);
	E.modified++;
}

void rowAppendString(erow *row, char *string, size_t len) {
	//Reallocate space for the current row + the new string
	row->chars = realloc(row->chars, row->size + len + 1);
	memcpy(&row->chars[row->size], string, len);

	//Add the length of the string to the row length
	row->size += len;
	//Add the null-terminator
	row->chars[row->size] = '\0';

	updateRow(row);
	E.modified++;
}

void freeRow(erow *row) {
	free(row->render);
	free(row->chars);
	free(row->hl);
}

void deleteRow(int at) {
	if (at < 0 || at >= E.numrows) {
		return;
	}

	//Free the space of the row
	freeRow(&E.row[at]);
	memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));

	for (int i = at; i < E.numrows - 1; i++) {
		E.row[i].idx--;
	}

	E.numrows--;
	E.modified++;
}
