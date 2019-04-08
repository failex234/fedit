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
	
	E.numrows++;
}