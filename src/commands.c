#include "fedit.h"

void parseCommandLine(const char *command) {
    unsigned int cmdlen = strlen(command);
    //Go through command
    for (unsigned int i = 0; i < cmdlen; i++) {
        if (i - 1 != cmdlen) {
            VIM.force = command[i + 1] == '!';
        }

        switch (command[i])
        {
            case 'w':
                file_save();
                break;
            case 'q':
                if (E.modified && !VIM.force) {
                    setStatusMessage(0, "Warning! file has unsaved changes");
                } else {
                    quit();
                }
                break;
            case 'h':
                setStatusMessage(0, "help menu not done yet");
                break;
            case '!':
                break;
            default:
                setStatusMessage(0, "command not recognized");
                break;
        }
    }
}
