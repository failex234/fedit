#include "fedit.h"

void updateSyntax(erow *row) {
    //Rellocate / allocate space for the line
    row->hl = realloc(row->hl, row->rsize);

    //Fill the highlight array with HL_NORMAL
    memset(row->hl, HL_NORMAL, row->rsize);
    
    //Check every character
    for (int i = 0; i < row->rsize; i++) {
        if (isdigit(row->render[i])) {
            row->hl[i] = HL_NUMBER;
        }
    }
}

int syntaxToColor(int hl) {
    switch(hl) {
        case HL_NUMBER:
            return 31;
        default:
            return 37;
    }
}