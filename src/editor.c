#include "fedit.h"
void rowInsertChar(erow *row, int at, int c) {
	if (at < 0 || at > row->size) {
		at = row->size;
	}
	
	row->chars = realloc(row->chars, row->size + 2);
	
	memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	
	row->size++;
	row->chars[at] = c;
	
	updateRow(row);
}

void insertChar(int c) {
	if (E.cy == E.numrows) {
		appendRow("", 0);
	}
	rowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}