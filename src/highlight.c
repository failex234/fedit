#include "highlight.h"

void updateSyntax(erow *row) {
    //Rellocate / allocate space for the line
    row->hl = realloc(row->hl, row->rsize);

    //Fill the highlight array with HL_NORMAL
    memset(row->hl, HL_NORMAL, row->rsize);

    if (E.syntax == NULL) {
        return;
    }

    char **keywords = E.syntax->keywords;

    char *scs = E.syntax->singleline_comment_start;
    int scs_len = scs ? strlen(scs) : 0;

    int prev_sep = 1;
    int in_string = 0;
    
    //Check every character
    int i = 0;
    while (i < row->rsize) {
        char c = row->render[i];
        unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

        //Handle comments
        if (scs_len && !in_string) {
            if (!strncmp(&row->render[i], scs, scs_len)) {
                memset(&row->hl[i], HL_COMMENT, row->rsize);
                break;
            }
        }

        //Handle quotes
        if (E.syntax->flags & HL_HIGHLIGHT_STRINGS) {
            if (in_string) {
                row->hl[i] = HL_STRING;

                //Check for escaped quotes
                if (c == '\\' && i + 1 < row->rsize) {
                    row->hl[i + 1] = HL_STRING;
                    i += 2;
                    continue;
                }
                if (c == in_string) {
                    in_string = 0;
                }
                i++;
                prev_sep = 1;

                continue;
            } else {
                if (c == '"' || c == '\'') {
                    in_string = c;
                    row->hl[i] = HL_STRING;
                    i++;
                    continue;
                }
            }
        }

        //Handle numbers
        if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
            if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) || (c == '.' && prev_hl == HL_NUMBER)) {
                row->hl[i] = HL_NUMBER;
                i++;
                prev_sep = 0;

                continue;
            }
        }

        if (prev_sep) {
            int j;
            for (j = 0; keywords[j]; j++) {
                int klen = strlen(keywords[j]);
                int kw2 = keywords[j][klen - 1] == '|';

                if (kw2) klen--;

                if (!strncmp(&row->render[i], keywords[j], klen) && isSeperator(row->render[i + klen])) {
                    memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
                    i += klen;
                    break;
                }
            }
            if (keywords[j] != NULL) {
                prev_sep = 0;
                continue;
            }

        }

        prev_sep = isSeperator(c);
        i++;
    }
}

int syntaxToColor(int hl) {
    switch(hl) {
        case HL_NUMBER:
            return 31;
        case HL_KEYWORD2:
            return 32;
        case HL_KEYWORD1:
            return 33;
        case HL_MATCH:
            return 34;
        case HL_STRING:
            return 35;
        case HL_COMMENT:
            return 36;
        default:
            return 37;
    }
}

void selectSyntaxHighlight() {
    E.syntax = NULL;

    //Don't bother if no file is open
    if (E.filename == NULL) {
        return;
    }

    //Get the file extension
    char *ext = strchr(E.filename, '.');

    for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
        //Get the syntax name
        struct editorSyntax *s = &HLDB[j];

        unsigned int i = 0;

        while(s->filematch[i]) {
            int is_ext = (s->filematch[i][0] == '.');

            if ((is_ext && ext && !strcmp(ext, s->filematch[i])) || (!is_ext && strstr(E.filename, s->filematch[i]))) {
                E.syntax = s;
                return;
            }
            i++;
        }
    }
}

int isSeperator(int c) {
    return (
        isspace(c)  ||
        c == '\0'   ||
        strchr(",.()+-/*=~%<>[];", c) != NULL
    );
}