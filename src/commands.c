#include <stdlib.h>
#include <string.h>


#include "commands.h"
#include "editor.h"
#include "file.h"
#include "highlighting.h"
#include "terminal.h"

void parse_command_prompt(char *prompt) {
    int hasForceFlag = find_force_flag(prompt);
    char *command_name = get_command(prompt, hasForceFlag);
    char *arguments = getArgument(prompt);

    if (command_name) {
        if (!strcmp(command_name, "w")) {
            if (arguments) {
                set_file(arguments);
            }
            file_save();
        } else if (!strcmp(command_name, "q")) {
            if (editorState.modified && !hasForceFlag) {
                setStatusMessage(0, "Warning! File has unsaved changed");
            } else {
                quit(0);
            }
        } else if (!strcmp(command_name, "wq")) {
            if (arguments) {
                set_file(arguments);
            }
            if (file_save()) {
                quit(0);
            }
        } else if (!strcmp(command_name, "x")) {
            if (file_save()) {
                quit(0);
            }
        } else if (!strcmp(command_name, "a")) {
            if (arguments) {
                setStatusMessage(0, "Argument: %s", arguments);
            } else {
                setStatusMessage(0, "No arguments given :(");
            }
        } else if (!strcmp(command_name, "h")) {
            setStatusMessage(0, "w = (w)rite file, q = (q)uit, x = write and e(x)it");
        } else if (strlen(command_name) > 4 && command_name[0] == '%' && command_name[1] == 's' && command_name[2] == '/') {

            //extract the "find" string
            char c;
            unsigned int i = 3;

            while ((c = command_name[i]) != '/' && i < strlen(command_name)) {
                i++;
            }

            int findLen = i - 3;
            char *find = malloc(findLen + 1);

            memcpy(find, command_name + 3, findLen);
            find[findLen] = '\0';

            i++;

            //extract the "substitute string"
            while ((c = command_name[i]) != '/' && i < strlen(command_name)) {
                i++;
            }

            //Check if arguments is in the expected format / doesn't contain errors
            if ((i - 1 == strlen(command_name) && command_name[i - 2] != '/') || count_chars(command_name, strlen(command_name), '/') < 3) {
                free(find);
                setStatusMessage(5, "Missing subsitute string!");

                return;
            } else if (strlen(find) == 0) {
                free(find);
                setStatusMessage(5, "Find string cannot be empty!");

                return;
            } else if (count_chars(command_name, strlen(command_name), '/') > 3) {
                //TODO Ignore escaped slashes
                free(find);
                setStatusMessage(5, "Invalid substitute syntax!");

                return;
            }

            int substLen = i + 1 - findLen - 5;
            char *subst = malloc(substLen + 1);

            memcpy(subst, command_name + findLen + 4, substLen);
            subst[substLen] = '\0';

            //extract the flags
            if ((int) strlen(command_name) > findLen + substLen + 5) {
                i = findLen + substLen + 5;

                while ((c = command_name[i]) != '\0') {
                    i++;
                }

                int flagsLen = strlen(command_name) - findLen - substLen - 4;
                char *flags = malloc(flagsLen + 1);

                memcpy(flags, command_name + 4 + findLen + substLen + 1, flagsLen);
                flags[flagsLen] = '\0';

                setStatusMessage(0, "Find: \"%s\", Replace with: \"%s\", Flags: \"%s\"", find, subst, flags);
                free(flags);
            } else {
                setStatusMessage(0, "Find: \"%s\", Replace with: \"%s\"", find, subst);
            }

            free(find);
            free(subst);
        } else if (!strcmp(command_name, "syntax")) {
            if (arguments) {
                if (!strcmp(arguments, "off")) {
                    setStatusMessage(0, "Syntax highlighting turned off");
                    forceSyntaxHighlighting(1, NULL);
                } else if (!strcmp(arguments, "on")) {
                    setStatusMessage(0, "Syntax highlighting turned on");
                    forceSyntaxHighlighting(0, NULL);
                } else {
                    struct editorSyntax *matchedSyntax = findSyntax(arguments);
                    if (matchedSyntax != NULL) {
                        forceSyntaxHighlighting(0, matchedSyntax);
                        setStatusMessage(0, "Syntax highlighting set to %s", arguments);
                    } else {
                        setStatusMessage(0, "Syntax name not found");
                    }
                }
            } else {
                setStatusMessage(0, "Usage: syntax <on|off|<syntaxname>>");
            }
        } else if (!strcmp(command_name, "num")) {
            if (arguments) {
                int enable_nums = !strcmp(arguments, "on");
                if (enable_nums && !editorState.disable_linenums) {
                    setStatusMessage(0, "Line numbers already enabled!");
                } else if (!enable_nums && editorState.disable_linenums) {
                    setStatusMessage(0, "Line numbers already disabled!");
                } else {
                    editorState.disable_linenums = !enable_nums;
                    refreshScreen();
                    if (enable_nums) setStatusMessage(0, "Line numbers enabled!");
                    else setStatusMessage(0, "Line numbers disabled!");
                }
            } else {
                setStatusMessage(0, "Usage: num <on|off>");
            }
        } else {
            setStatusMessage(0, "Command not recognized");
        }

        free(command_name);
        free(prompt);

        if (arguments) free(arguments);
    } else {
        setStatusMessage(0, "Command not recognized %s");
    }
}

int find_force_flag(char *string) {
    int sepfound = 0;
    int idx = 0;

    //Look for the force flag (!). Only use the
    //found flag when it is part of the command
    for (unsigned int i = 0; i < strlen(string); i++) {
        if (string[i] == '!' && !sepfound) {
            idx = (signed int) i;
        } else if (string[i] == ' ') {
            sepfound = 1;
        }
    }

    return idx;
}

char *get_command(char *string, int hasForce) {
    char *command;
    char splitchar;
    uint len = strlen(string);
    int sepfound = 0;
    int idx = 0;

    //If user is forcing a command we want to look for the ! instead of a space
    if (hasForce) {
        splitchar = '!';
    } else {
        splitchar = ' ';
    }

    for (uint i = 0; i < len; i++) {
        //Make sure that we find the splitchar before a space
        if (!sepfound && string[i] == splitchar) {
            idx = i;
            break;
        }
        if (string[i] == ' ') {
            sepfound = 1;
        }
    }

    //If there are no arguments, copy the command over and add a null-terminator
    if (!idx && len > 0) {
        command = malloc(len + 1);
        memcpy(command, string, len);
        command[len] = '\0';
    } else {
        command = malloc(idx + 1);
        memcpy(command, string, idx);
        command[idx] = '\0';
    }

    return command;
}

char *getArgument(char *string) {
    char *argument;
    char *newarg;
    unsigned int len;

    //Check if string contains a space
    argument = strstr(string, " ");

    if (!argument) {
        return NULL;
    }

    //Copy the characters behind the first space into a new string
    len = strlen(argument) - 1;
    newarg = malloc(len + 1);

    memcpy(newarg, argument + 1, len);
    newarg[len] = '\0';

    return newarg;

}


int count_chars(const char *string, int len, char find) {
    int count = 0;

    //Go through the string an count every encounter of the given character
    for (int i = 0; i < len; i++) {
        if (string[i] == find) count++;
    }

    return count;
}
