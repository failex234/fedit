#ifndef FLX_SYNHIGH_H
#define FLX_SYNHIGH_H
#include "fedit.h"

char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL};
char *JAVA_HL_extentions[] = { ".java", NULL };

char *C_HL_keywords[] = {
	"switch", "if", "while", "for", "break", "continue", "return", "else",
	"struct", "union", "typedef", "enum", "class", "case", "volatile", "extern",
	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|", "void|", NULL
};

char *JAVA_HL_keywords[] = {
	"abstract", "assert", "break", "case", "catch", "class", "const", "default", "do",
	"else", "enum", "extends", "final", "finally", "for", "goto", "if", "implements",
	"import", "instanceof", "interface", "native", "package", "private", "protected",
	"public", "return", "static", "strictfp", "super", "switch", "synchronized", "this",
	"throw", "throws", "transient", "try", "void", "volatile", "while", "continue",
	"boolean|", "byte|", "char|", "double|", "float|", "int|", "long|", "short|",
	"true<", "false<", "new<", "null<", NULL
};

struct editorSyntax HLDB[] = {
 {
	 "C",
	 C_HL_extensions,
	 C_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "Java",
	 JAVA_HL_extentions,
	 JAVA_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
};


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif