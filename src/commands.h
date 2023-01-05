#ifndef commands_h
#define commands_h

///Parse and execute entered commands
void parse_command_prompt(char *prompt);
///Look for vim force flag in string (!)
int find_force_flag(char *string);
///Extract command from prompt
char *get_command(char *string, int has_force_flag);
///Extract arguments from prompt
char *getArgument(char *string);
///Count number of character occurences in string
int count_chars(const char *string, int string_length, char search_chr);


#endif /* commands_h */
