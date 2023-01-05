#ifndef commands_h
#define commands_h

void parseCommandLine(char *);
int parseForceFlag(char *);
char *getCommand(char *, int);
char *getArgument(char *);
int countChars(const char *, int, char);


#endif /* commands_h */
