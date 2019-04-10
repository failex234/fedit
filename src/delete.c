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


void deleteNWords(int number, int end) {
    //TODO
    UNUSED(number);
    UNUSED(end);
}