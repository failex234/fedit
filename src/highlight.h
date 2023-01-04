#ifndef FLX_SYNHIGH_H
#define FLX_SYNHIGH_H
#include "fedit.h"

char *C_HL_extensions[] = { ".c", ".h", ".cpp", ".hpp", ".C", ".H", ".CPP", ".cc", ".CC", ".cxx", ".CXX", NULL };
char *CS_HL_extension[] = { ".cs", ".CS", NULL };
char *JAVA_HL_extensions[] = { ".java", ".JAVA", NULL };
char *JAVASCRIPT_HL_extensions[] = { ".js", ".JS", NULL };
char *PHP_HL_extensions[] = { ".php", ".PHP", NULL };
char *PY_HL_extensions[] = { ".py", ".PY", NULL };
char *VB_HL_extensions[] = { ".vb", ".vba", ".VB", ".VBA", NULL };
char *BASH_HL_extensions[] = { ".sh", NULL };
char *OBJC_HL_extensions[] = { ".m", ".M", ".mm", ".MM", NULL };
char *KOT_HL_extensions[] = { ".kt", ".KT", NULL };


char *C_HL_keywords[] = {
        "auto", "restrict", "static", "default", "inline", "register", "sizeof",
        "switch", "if", "while", "for", "break", "continue", "return", "else",
        "struct", "union", "typedef", "enum", "class", "case", "volatile", "extern",
        "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|", "void|",
        "_Bool|", "_Complex|", "_Imaginary|", "FILE|", "size_t|", "time_t|",
        "ssize_t|", "uint|", "int8_t|", "int16_t|", "int32_t|", "int64_t|", "uint8_t|",
        "uint16_t|", "uint32_t|", "uint64_t|", "uid_t|", "bool|", "wchar_t|",
        "-><", "NULL<", "EOF<", "SEEK_SET<", "SEEK_END<", "SEEK_CUR<", "true|", "false|",
        "errno|", "EXIT_FAILURE|", "EXIT_SUCCESS|",
        "stdin<", "stdout<", "stderr<",  "#define<",
        "#undef<", "#ifdef<", "#ifndef<", "#if<", "#else<", "#elif<", "#endif<",
        "#line<", "__FILE__<", "__FILE_NAME__<", "__LINE__<", "__DATE__<",
        "__TIME__<", "__TIMESTAMP__<", "__func__<", "__FUNCTION__<",
        "__PRETTY_FUNCTION__<", "#pragma<", "#error<", "#warning<", "#include<", NULL
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
        "var|", "let|", "await",  "break", "case", "catch", "continue", "debugger", "default",
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

char *PY_HL_keywords[] = {
        "and", "as", "assert", "break", "class", "continue", "def", "del", "elif",
        "else", "except", "finally", "for", "from", "global", "if", "import", "in",
        "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try",
        "while", "with", "yield", "True<", "False<", "None<", NULL
};

char *VB_HL_keywords[] = {
        "AddHandler", "AddressOff", "Alias", "And", "AndAlso", "As", "Call",
        "Case", "Catch", "Const", "Continue", "Declare", "Default", "Delegate",
        "Dim", "DirectCast", "Do", "Each", "Else", "ElseIf", "End", "EndIf",
        "Enum", "Erase", "Error", "Event", "Exit", "Finally", "For", "Friend",
        "Function", "Get", "GetType", "Global", "GoSub", "GoTo", "Handles",
        "If", "Implements", "Imports", "In", "Inherits", "Interface", "Is",
        "IsNot", "Let", "Lib", "Like", "Loop", "Mod", "Module", "MustInherit",
        "MustOverride", "Namespace", "Narrowing", "Next", "Not", "NotInheritable",
        "NotOverridable", "Of", "On", "Operator", "Option", "Optional", "Or",
        "OrElse", "Out", "Overloads", "Overridable", "Overrides", "ParamArray",
        "Partial", "Private", "Property", "Protected", "Public", "RaiseEvent",
        "ReadOnly", "ReDim", "REM", "RemoveHandler", "Resume", "Return", "Select",
        "Set", "Shadows", "Shared", "Static", "Step", "Stop", "Structure", "Sub",
        "SyncLock", "Then", "Throw", "To", "Try", "TryCast", "TypeOf", "Using",
        "When", "While", "Widening", "With", "WithEvents", "WriteOnly", "Xor",
        "#Const", "#Else", "#ElseIf", "#End", "#If",
        "Byte|", "Decimal|", "Long|", "ULong|", "Integer|", "SByte|", "Boolean|",
        "Short|", "Double|", "String|", "Variant|", "Date|", "Single|", "Object|",
        "True<", "False<", "Nothing<", "CBool<", "CByte<", "CChar<",
        "CDate<", "CDbl<", "CDec<", "CInt<", "CLng<", "CObj<", "CSByte<",
        "CShort<", "CSng<", "CStr<", "CUInt<", "CULng<", "CUShort<",
        "Me<", "MyBase<", "MyClass<", "New<", "ByRef<", "ByVal<", NULL
};

char *BASH_HL_keywords[] = {
        "if", "fi", "do", "done", "for", "else", "elif", "[|", "]|", "[[|",
        "]]|", "alias|", "read|", "echo|", "$|", "printf", "sudo", "case",
        "in", "esac", "mount", "uname", "ls", "ps", "make", "cc", "gcc",
        "clang", "automake", "autoconf", "umount", "cd", "pwd", "$PWD|",
        "$@|", "$!|", "$1|", "$2|", "$3|", "$4|", "touch", "ld", "which",
        "dir", "whoami", "$UID|", "who", "grep", "awk", "cat", "sed", NULL
};

char *OBJC_HL_keywords[] = {
        "asm", "auto", "break", "bycopy", "byref", "case", "const",
        "continue", "default", "do", "else", "enum", "extern", "for",
        "goto", "if", "inline", "register", "restrict", "return", "sizeof",
        "static", "struct", "switch", "typedef", "union", "volatile", "while",
        "in", "In", "inout", "nil", "Nil", "YES", "NO", "NULL", "oneway",
        "out", "SEL", "self", "super", "atomic", "nonatomic", "copy", "strong",
        "weak", "retrain", "__strong", "__weak", "__autoreleasing",
        "__unsafe_unretained", "_cmd", "char|", "double|", "float|", "int|",
        "long|", "short|", "signed|", "unsigned|", "void|", "_Bool|", "BOOL|",
        "_Complex|", "_Imaginary|", "BOOL|", "id|", "instancetype|", "IMP|",
        "NSString|", "NSObject|", "NSArray|", "NSMutableString|", "NSInteger|",
        "NSUInteger|", "NSNumber|", "NSError|", "Protocol<", "Class<", "@interface<",
        "@end<", "@implementation<", "@protocol<", "@class<", "@public<",
        "@protected<", "@private<", "@property<", "@try<", "@throw<", "@catch<",
        "@finally<", "@synthesize<", "@dynamic<", "@selector<", "#define<",
        "#undef<", "#ifdef<", "#ifndef<", "#if<", "#else<", "#elif<", "#endif<",
        "#line<", "__FILE__<", "__FILE_NAME__<", "__LINE__<", "__DATE__<",
        "__TIME__<", "__TIMESTAMP__<", "__func__<", "__FUNCTION__<",
        "__PRETTY_FUNCTION__<", "#pragma<", "#error<", "#warning<",
        "#include<", "#import<", NULL
};

char *KOT_HL_keywords[] = {
        "as","break","continue","do","else","false","for","fun","if",
        "in","is","null","return","super","this","throw","true","try",
        "typealias","typeof","val","var","when","while","by","catch",
        "constructor","field","field","finally","get","init","param",
        "lateinit","final","open","out","override","private","protected",
        "public","reified","sealed","suspend","expect","internal","infix",
        "inline","inner","dynamic","crossinline","data","tailrec","vararg",
        "it","import<","class<","object<","companion<","delegate<","package<",
        "interface<","abstract<","actual<","annotation<","const<","enum<",
        "expect<","int|","boolean|","long|","short|","char|","unsigned|",
        "signed|","String|","Array|","HashMap|", "operator", NULL
};



struct editorSyntax HLDB[] = {
        {
                "C/C++",
                C_HL_extensions,
                C_HL_keywords,
                "//", "/*", "*/",
                HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HIGHLIGHT_LTGT
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
        },
        {
                "Visual Basic",
                VB_HL_extensions,
                VB_HL_keywords,
                "'", NULL, NULL,
                HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
        },
        {
                "Bash",
                BASH_HL_extensions,
                BASH_HL_keywords,
                "#", NULL, NULL,
                HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
        },
        {
                "Objective-C",
                OBJC_HL_extensions,
                OBJC_HL_keywords,
                "//", "/*", "*/",
                HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS | HL_HIGHLIGHT_LTGT
        },
        {
                "Kotlin",
                KOT_HL_extensions,
                KOT_HL_keywords,
                "//", "/*", "*/",
                HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
        }
};


#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif
