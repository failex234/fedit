#include "highlight.h"

void updateSyntax(erow *row) {
    //Rellocate / allocate space for the line
    row->highlight_types = realloc(row->highlight_types, row->render_length);

    //Fill the highlight array with HL_NORMAL
    memset(row->highlight_types, HL_NORMAL, row->render_length);

    if (editorState.syntax == NULL) {
        return;
    }

    char **keywords = editorState.syntax->keywords;

    char *scs = editorState.syntax->singleline_comment_start;
    char *mcs = editorState.syntax->multiline_comment_start;
    char *mce = editorState.syntax->multiline_comment_end;

    int scs_len = scs ? strlen(scs) : 0;
    int mcs_len = mcs ? strlen(mcs) : 0;
    int mce_len = mce ? strlen(mce) : 0;

    int prev_sep = 1;
    int in_string = 0;
    int in_ltgt = 0;
    int in_templ = 0;
    int in_format = 0;
    int in_comment = (row->idx > 0 && editorState.rows[row->idx - 1].hl_open_comment);

    //Check every character
    int i = 0;
    while (i < row->render_length) {
        char c = row->render[i];
        unsigned char prev_hl = (i > 0) ? row->highlight_types[i - 1] : HL_NORMAL;

        //Handle comments
        if (scs_len && !in_string && !in_comment) {
            if (!strncmp(&row->render[i], scs, scs_len)) {
                memset(&row->highlight_types[i], HL_COMMENT, row->render_length - i);
                break;
            }
        }

        //Handle multiline comments
        if (mcs_len && mce_len && !in_string) {
            if (in_comment) {
                row->highlight_types[i] = HL_MLCOMMENT;

                if (!strncmp(&row->render[i], mce, mce_len)) {
                    memset(&row->highlight_types[i], HL_MLCOMMENT, mce_len);

                    i += mce_len;
                    in_comment = 0;
                    prev_sep = 1;

                    continue;
                } else {
                    i++;
                    continue;
                }
            } else if (!strncmp(&row->render[i], mcs, mcs_len)) {
                memset(&row->highlight_types[i], HL_MLCOMMENT, mcs_len);
                i += mcs_len;
                in_comment = 1;
                continue;
            }
        }

        //Handle string templates ("Hello $name!")
        if (editorState.syntax->flags & HL_HIGHLIGHT_TEMPLATES && in_string) {
            if (c == '$') {
                in_templ = 1;
                row->highlight_types[i++] = HL_TEMPLATE_FORMAT;
                continue;
            } else if (in_templ && !is_seperator(c)) {
                row->highlight_types[i++] = HL_TEMPLATE_FORMAT;
                if (c == '}') {
                    in_templ = 0;
                }
                continue;
            } else {
                in_templ = 0;
            }
        }

        //Handle format specifiers ("Hello %s!")
        if (editorState.syntax->flags & HL_HIGHLIGHT_PERCENT_FORMAT_SP && in_string) {
            if (c == '%') {
                in_format = 1;
                row->highlight_types[i++] = HL_TEMPLATE_FORMAT;
                continue;
            } else if (in_format && !is_format_seperator(c)) {
                row->highlight_types[i++] = HL_TEMPLATE_FORMAT;
                continue;
            } else {
                in_format = 0;
            }
        }

        //Handle quotes
        if (editorState.syntax->flags & HL_HIGHLIGHT_STRINGS) {
            //Check for escpaed quotes
            if (c == '"' && i > 0 && row->render[i - 1] == '\\') {
                i += 2;
                continue;
            }
            if (in_string) {
                row->highlight_types[i] = HL_STRING;

                if (c == in_string) {
                    in_string = 0;
                }
                i++;
                prev_sep = 1;

                continue;
            } else {
                if (c == '"' || c == '\'') {
                    in_string = c;
                    row->highlight_types[i] = HL_STRING;
                    i++;
                    continue;
                }
            }
        }

        //Check for ltgt blocks (< >)
        if (editorState.syntax->flags & HL_HIGHLIGHT_LTGT) {
            if (c == '<' && !in_ltgt && !in_string && row->render[row->render_length - 1] == '>') {
                row->highlight_types[i] = HL_STRING;
                in_ltgt = 1;
                i++;
                continue;
            } else if (c == '>' && in_ltgt && !in_string) {
                row->highlight_types[i] = HL_STRING;
                in_ltgt = 0;
                i++;
                continue;
            } else if (in_ltgt) {
                row->highlight_types[i] = HL_STRING;
                i++;
                continue;
            }
        }

        //Handle numbers
        if (editorState.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
            if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) || (c == '.' && prev_hl == HL_NUMBER)) {
                row->highlight_types[i] = HL_NUMBER;
                i++;
                prev_sep = 0;

                continue;
            }
        }

        if (prev_sep) {
            int j;
            for (j = 0; keywords[j]; j++) {
                size_t klen = strlen(keywords[j]);
                int kw2 = keywords[j][klen - 1] == '|';
                int kw3 = keywords[j][klen - 1] == '<';

                if (kw2 ||kw3) klen--;

                if (!strncmp(&row->render[i], keywords[j], klen) && is_seperator(row->render[i + klen])) {
                    if (kw2) {
                        memset(&row->highlight_types[i], HL_KEYWORD2, klen);
                    } else if (kw3) {
                        memset(&row->highlight_types[i], HL_KEYWORD3, klen);
                    } else {
                        memset(&row->highlight_types[i], HL_KEYWORD1, klen);
                    }
                    i += (int) klen;
                    break;
                }
            }
            if (keywords[j] != NULL) {
                prev_sep = 0;
                continue;
            }

        }

        prev_sep = is_seperator(c);
        i++;
    }

    int changed = (row->hl_open_comment != in_comment);
    row->hl_open_comment = in_comment;

    if (changed && row->idx + 1 < editorState.numrows) {
        updateSyntax(&editorState.rows[row->idx + 1]);
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
        case HL_TEMPLATE_FORMAT:
            return 34;
        case HL_KEYWORD3:
        case HL_STRING:
            return 35;
        case HL_MLCOMMENT:
        case HL_COMMENT:
            return 36;
        default:
            return 37;
    }
}

void determineSyntaxHighlight() {
    editorState.syntax = NULL;

    //Don't bother if no file is open
    if (editorState.filename == NULL) {
        return;
    }

    //Get the file extension
    char *ext = strrchr(editorState.filename, '.');

    for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
        //Get the syntax name
        struct editorSyntax *s = &HLDB[j];

        unsigned int i = 0;

        while(s->filematch[i]) {
            int is_ext = (s->filematch[i][0] == '.');

            if ((is_ext && ext && !strcmp(ext, s->filematch[i])) || (!is_ext && strstr(editorState.filename, s->filematch[i]))) {
                editorState.syntax = s;

                for (int filerow = 0; filerow < editorState.numrows; filerow++) {
                    updateSyntax(&editorState.rows[filerow]);
                }

                return;
            }
            i++;
        }
    }
}

void forceSyntaxHighlighting(int disable, struct editorSyntax *syntaxHighlighting) {
    if (disable && syntaxHighlighting == NULL) {
        editorState.disable_highlight = 1;
        refreshScreen();
    } else if (!disable && syntaxHighlighting == NULL) {
        editorState.disable_highlight = 0;
        refreshScreen();
    } else if (syntaxHighlighting != NULL){
        editorState.syntax = syntaxHighlighting;
        for (int filerow = 0; filerow < editorState.numrows; filerow++) {
            updateSyntax(&editorState.rows[filerow]);
        }
    }
}

int is_seperator(int c) {
    return (
            isspace(c)  ||
            c == '\0'   ||
            strchr(",.()+-/*=~%<>[];:!?#\\", c) != NULL
    );
}

int is_format_seperator(int c) {
    return (
            isspace(c)  ||
            c == '\0'   ||
            strchr(",()+-/*=~%<>[];:!?#\\", c) != NULL
    );
}

struct editorSyntax *findSyntax(const char *syntaxname) {
    for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
        struct editorSyntax *s = &HLDB[j];
        if (!strcmp(s->syntaxname, syntaxname)) return s;
    }
    return NULL;
}