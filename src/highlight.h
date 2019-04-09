#ifndef FLX_SYNHIGH_H
#define FLX_SYNHIGH_H
#include "fedit.h"

char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL};
char *JAVA_HL_extentions[] = { ".java", NULL };
char *JAVASCRIPT_HL_extentions[] = { ".js", NULL };
char *PHP_HL_extentions[] = { ".php", NULL };


char *C_HL_keywords[] = {
	"switch", "if", "while", "for", "break", "continue", "return", "else",
	"struct", "union", "typedef", "enum", "class", "case", "volatile", "extern",
	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|", "void|", NULL
};

char *JAVA_HL_keywords[] = {
	"abstract", "assert", "break", "case", "catch", "class", "continue", "const", "default", "do",
	"else", "enum", "extends", "final", "finally", "for", "goto", "if", "implements",
	"import", "instanceof", "interface", "native", "package", "private", "protected",
	"public", "return", "static", "strictfp", "super", "switch", "synchronized", "this",
	"throw", "throws", "transient", "try", "void", "volatile", "while", "continue",
	"boolean|", "byte|", "char|", "double|", "float|", "int|", "long|", "short|",
	"true<", "false<", "new<", "null<", NULL
};

char *JAVASCIRPT_HL_keywords[] = {
	"var", "let", "await",  "break", "case", "catch", "continue", "debugger", "default",
	"delete", "do", "else", "finally", "for", "function", "if", "in", "instanceof", 
	"return", "switch", "throw", "try", "typeof", "void", "while", "with", "enum", "export",
	"extends", "import", "super", "implements", "interface", "package", "private", "protected",
	"constructor", "public", "static", "yield", "true<", "false<", "new<", "null<", "NaN<",
	"undefined<", "this<", "=><", NULL
};

char *PHP_HL_keywords[] = {
	"__halt_compiler|", "abstract", "array|", "as", "break",
	"callable", "case", "catch", "class", "clone", "const",
	"continue", "declare", "default", "die|", "do", "echo", "else",
	"elseif", "empty", "enddeclare", "endfor", "endforeach", "endif",
	"endswitch", "endwhile", "eval|", "exit|", "extends", "final",
	"finally", "for", "foreach", "function", "global", "goto", "if",
	"implements", "include" "include_once", "instanceof", "insteadof",
	"interface", "isset|", "list|", "namespace", "print", "private",
	"protected", "public", "require", "require_once", "return", "static",
	"switch", "throw", "trait", "try", "unset|", "use", "while", "yield",
	"yield from", "new<", "var<", "true<", "false<", "or<", "xor<", "and<", NULL
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
 {
	 "JavaScript",
	 JAVASCRIPT_HL_extentions,
	 JAVASCIRPT_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "PHP",
	 PHP_HL_extentions,
	 PHP_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
};


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif