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
	char *prearg;
	char *argument;
	
	int len = strlen(string);
	int seppos = 0;
	
	//Find the space between command and argument
	for (unsigned int i = 0; i < len; i++) {
		if (string[i] == ' ' && string[i + 1] != NULL) {
			seppos = (signed int) i;
			break;
		}
	}
	
	if(!seppos) {
		return NULL;
	}
	
	//Let prearg point to the character behind the space
	prearg = *string + seppos + 1;
	argument = malloc(len - seppos);
	
	//Copy everything behind the space into argument char
	memcpy(argument, prearg, len - seppos);
	
	return argument;
	
}