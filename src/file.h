#ifndef file_h
#define file_h

///File name of the file to be read. When given via cmdline argument
char *cmdline_openfile;

void file_open(char *);
int file_save();
char *rows_to_string(int *);
void set_file(const char *);

#endif /* file_h */
