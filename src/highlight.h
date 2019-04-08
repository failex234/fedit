#ifndef FLX_SYNHIGH_H
#define FLX_SYNHIGH_H
#include "fedit.h"

char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL};
char *C_HL_keywords[] = {
	"switch", "if", "while", "for", "break", "continue", "return", "else",
	"struct", "union", "typedef", "enum", "class", "case", "volatile", "extern",
	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|", "void|", NULL
};

struct editorSyntax HLDB[] = {
 {
	 "C",
	 C_HL_extensions,
	 C_HL_keywords,
	 "//",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
};


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif