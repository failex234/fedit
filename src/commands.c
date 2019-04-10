#include "fedit.h"

void parseCommandLine(const char *command) {
    int hasForceFlag = parseForceFlag(command);
	char *cmdonly = getCommand(command, hasForceFlag);
	
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
		
	} else if (!strcmp(cmdonly, "h")) {
		setStatusMessage(0, "No help menu yet :(");
	} else {
		setStatusMessage(0, "Command not recognized");
	}
	
	free(command);
}

int parseForceFlag(const char *string) {
	int sep_found = 0;
	int idx = 0;
	for (unsigned int i = 0; i < strlen(string); i++) {
		if (string[i] == '!' && !sepfound) {
			idx = (signed int) i;
		} else if (string[i] == ' ') {
			sep_found = 1;
		}
	}
	
	return idx;
}

char *getCommand(const char *string, int hasForce) {
	char *command;
	if (hasForce) {
		command = strtok(string, "!");
	} else {
		command = strtok(string, " ");
	}
	
	return command;
}

char *getArgument(const char *string) {
	
}