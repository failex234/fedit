#include "fedit.h"

void addToDeleteWords(char numAsChar) {
	if (!isdigit(numAsChar)) return;
	VIM.delwords = realloc(VIM.delwords, ++VIM.delwordsSize + 1);
	VIM.delwords[VIM.delwordsSize - 1] = numAsChar;
	VIM.delwords[VIM.delwordsSize] = '\0';
}


void deleteNWords(int number) {
	int spaces = 0;
	int lastchar = 0;
	int startidx = 0;
	int endidx = 0;
	int i = 0;
	
	if (E.cy > E.numrows || E.numrows == 0) {
		return;
	}
	
	char *currRow = E.row[E.cy].chars;
	int rowSize = E.row[E.cy].size;
	
	//Find the last space before the word
	for (i = 0; i < E.cx; i++) {
		if (currRow[i] == ' ') {
			startidx = i;
		}
	}
	
	if (E.cx) {
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
	
	deleteRow(E.cy);
	insertRow(E.cy, newrow, startidx + rowSize - endidx - 1);
	
	if (E.cx > E.row[E.cy].size) E.cx = E.row[E.cy].size - 1;

}