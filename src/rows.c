#include "fedit.h"
void appendRow(char *s, size_t len) {
	//Reallocate space for the current number of rows + a new row
	E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
	
	//Determine the last index
	int at = E.numrows;
	
	//Set the length and allocate space for the string that should be appended
	E.row[at].size = len;
	E.row[at].chars = malloc(len + 1);
	
	//Copy the string to the correct location
	memcpy(E.row[at].chars, s, len);
	
	//null terminate the string
	E.row[at].chars[len] = '\0';
	
	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	updateRow(&E.row[at]);
	
	E.numrows++;
}

//Replaces Tabs with 8 spaces to prevent "ghosting" of characters
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