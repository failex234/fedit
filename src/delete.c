#include "fedit.h"

//Add each digit to the current
//Number of to be deleted words
//So the calucation should look
//like this: 10 + 1 = 101
void addAsDigit(int number) {
    int currwords = VIM.delwords;
    currwords *= 10 + number;

    VIM.delwords = currwords;
}


void deleteNWords(int number) {
	int spaces = 0;
	int startidx = 0;
	int endidx = 0;
	int i = 0;
	
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
		if (currRow[i] == ' ') {
			spaces++;
		}
		i++;
	}
	
	endidx = i - 1;
	
	char *newrow = malloc(startidx + (rowSize - endidx));
	memcpy(newrow, currRow, startidx);
	memcpy(&newrow[startidx], &currRow[endidx + 1], rowSize - endidx - 1);
	
	newrow[startidx + rowSize - endidx - 1] = '\0';
	
	deleteRow(E.cy);
	insertRow(E.cy, newrow, startidx + rowSize - endidx - 1);	

}