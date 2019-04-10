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
    char *currrow = E.row[E.cy].chars;

    int currPos = E.cx;
    int rowSize = E.row[E.cy].size;
    int i = currPos;
    int endidx;

    //Look for the last space
    while (i < rowSize && number != spaces) {
        if (currrow[i] == ' ') {
            spaces++;
        }
        i++;
    }

    if (spaces != number) {
        endidx = rowSize;
    } else {
        endidx = i;
    }


    //Replace the current line with all the characters
    //behind the word when the cursor is at position 0
    if (!currPos) {
        char *behind = malloc(rowSize - endidx);
        memcpy(behind, &currrow[endidx], rowSize - endidx);
        behind[rowSize - endidx] = '\0';

        deleteRow(E.cy);
        insertRow(E.cy, behind, rowSize - endidx);
    } else if (currPos != rowSize - 1) {
    }

}