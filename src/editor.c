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
	E.modified++;
}

void rowDeleteChar(erow *row, int at) {
	if (at < 0 || at >= row->size) {
		return;
	}

	memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
	row->size--;

	updateRow(row);
	E.modified++;
}

void insertChar(int c) {
	if (E.cy == E.numrows) {
		appendRow("", 0);
	}
	rowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}

void deleteChar() {
	if (E.cy == E.numrows) {
		return;
	}

	erow *row = &E.row[E.cy];
	if (E.cx > 0) {
		rowDeleteChar(row, E.cx - 1);
		E.cx--;
	}
}