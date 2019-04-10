#ifndef FLX_SYNHIGH_H
#define FLX_SYNHIGH_H
#include "fedit.h"

char *C_HL_extensions[] = { ".c", ".h", ".cpp", NULL};
char *CS_HL_extension[] = { ".cs", NULL };
char *JAVA_HL_extensions[] = { ".java", NULL };
char *JAVASCRIPT_HL_extensions[] = { ".js", NULL };
char *PHP_HL_extensions[] = { ".php", NULL };
char *PY_HL_extensions[] = { ".py", NULL };


char *C_HL_keywords[] = {
	"switch", "if", "while", "for", "break", "continue", "return", "else",
	"struct", "union", "typedef", "enum", "class", "case", "volatile", "extern",
	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|", "void|",
	"-><", NULL
};

char *CS_HL_keywords[] = {
	"abstract", "const", "event", "override", "partial", "readonly",
	"sealed", "static", "unsafe", "virtual", "volatile", "public", "private", "internal",
	"protected", "if", "else", "switch", "case", "do", "for", "foreach", "in", "while",
	"break", "continue", "default", "goto", "return", "yield", "throw", "try", "catch",
	"finally", "fixed", "lock", "as", "bool|", "byte|", "char|", "class|", "decimal|",
	"double|", "enum|", "float|", "int|", "long|", "sbyte|", "short|", "string|",
	"struct|", "uint|", "ulong|", "ushort|", "::<", "=><", "as<", "await<", "async<",
	"new<", "null<", "true<", "false<", "value<", "void<", "base<", "this<", "using<",
	"extern<", "params<", "ref<", "out<", "is<", "sizeof<", "typeof<", "stackalloc<",
	"check<", "unchecked<", NULL
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
	"yield from", "new<", "var<", "true<", "false<", "or<", "xor<", "and<",
	"$<", NULL
};

char *PY_HL_keywords[] {
	"and", "as", "assert", "break", "class", "continue", "def", "del", "elif",
	"else", "except", "finally", "for", "from", "global", "if", "import", "in",
	"is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try",
	"while", "with", "yield", "True<", "False<", "None<", NULL
};

struct editorSyntax HLDB[] = {
 {
	 "C/C++",
	 C_HL_extensions,
	 C_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	"C#",
	CS_HL_extension,
	CS_HL_keywords,
	"//", "/*", "*/",
	HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "Java",
	 JAVA_HL_extensions,
	 JAVA_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "JavaScript",
	 JAVASCRIPT_HL_extensions,
	 JAVASCIRPT_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "PHP",
	 PHP_HL_extensions,
	 PHP_HL_keywords,
	 "//", "/*", "*/",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 },
 {
	 "Python",
	 PY_HL_extensions,
	 PY_HL_keywords,
	 "#", "\"\"\"", "\"\"\"",
	 HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
 }
};


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif