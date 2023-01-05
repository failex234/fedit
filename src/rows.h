#ifndef rows_h
#define rows_h

///Abstract representation of a rows in the editor
struct erow {
    ///Rows index in the whole file
    int idx;
    ///Character length of the row
    int length;
    ///Length of the rendered contents meant to be displayed
    int render_length;
    ///Raw sequence of the line contents
    char *chars;
    ///Raw sequence of the rendered contents
    char *render;
    ///Syntax highlighting type per character
    unsigned char *highlight_types;
    ///This line is part of a comment
    int hl_open_comment;
};

void insertRow(int, char *, size_t);
void updateRow(struct erow *row);
void rowInsertChar(struct erow *, int, int);
void rowDeleteChar(struct erow *, int);
void rowAppendString(struct erow *, char *, size_t);
int rowCxToRx(struct erow *, int);
int rowRxToCx(struct erow *, int);
void freeRow(struct erow *);
void deleteRow(int);

#endif /* rows_h */
