#include "fedit.h"

void parseCommandLine(char *command) {
    int hasForceFlag = parseForceFlag(command);
	char *cmdonly = getCommand(command, hasForceFlag);
	char *argument = getArgument(command);
	
	if (!strcmp(cmdonly, "w")) {
		file_save();
	} else if (!strcmp(cmdonly, "q")) {
		if (E.modified && !hasForceFlag) {
			setStatusMessage(0, "Warning! File has unsaved changed");
		} else {
			quit();
		}
	} else if (!strcmp(cmdonly, "wq")) {
		file_save();
		quit();
	} else if (!strcmp(cmdonly, "a")) {
		if (argument) {
			setStatusMessage(0, "Argument: %s", argument);
		} else {
			setStatusMessage(0, "No argument given :(");
		}
	} else if (!strcmp(cmdonly, "h")) {
		setStatusMessage(0, "No help menu yet :(");
	} else {
		setStatusMessage(0, "Command not recognized");
	}
	
	free(command);
}

int parseForceFlag(char *string) {
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

char *getCommand(char *string, int hasForce) {
	char *command;
	
	if (hasForce) {
		command = strtok(string, "!");
	} else {
		command = strtok(string, " ");
	}
	
	return command;
}

char *getArgument(char *string) {
	char *argument;
	argument = strstr(string, " ");
	
	if (!argument) {
		return NULL;
	}
	
	argument++;
	
	return argument;
	
}