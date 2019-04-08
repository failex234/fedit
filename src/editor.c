#include "fedit.h"
void insertChar(int c) {
	if (E.cy == E.numrows) {
		insertRow(E.numrows, "", 0);
	}
	rowInsertChar(&E.row[E.cy], E.cx, c);
	E.cx++;
}

void deleteChar() {
	if (E.cy == E.numrows || (E.cx == 0 && E.cy == 0)) {
		return;
	}

	erow *row = &E.row[E.cy];
	if (E.cx > 0) {
		rowDeleteChar(row, E.cx - 1);
		E.cx--;
	} else {
		//Append the text of the current line to the line above and then delete the current lin
		E.cx = E.row[E.cy - 1].size;

		rowAppendString(&E.row[E.cy - 1], row->chars, row->size);
		deleteRow(E.cy);

		E.cy--;
	}
}

void insertNewLine() {
	if (E.cx == 0) {
		//insert a blank line when we're at the beginning of a line
		insertRow(E.cy, "", 0);
	} else {
		erow *row = &E.row[E.cy];

		insertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);

		//Update the new row
		row = &E.row[E.cy];
		row->size = E.cx;
		row->chars[row->size] = '\0';

		updateRow(row);
	}
	E.cy++;
	E.cx = 0;
}