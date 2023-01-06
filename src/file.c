#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "editor.h"
#include "file.h"
#include "highlighting.h"
#include "rows.h"

void file_open(char *filename) {
    free(editorState.filepath);
    //TODO: replaced
    set_file(strdup(filename));

    determineSyntaxHighlight();

    FILE *fp = fopen(filename, "r");

    //Kill program when file doesn't exist
    if (!fp) {
        //TODO: replaced
        return;
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    //Read line and cut off carriage-return or newline
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r')) {
            linelen--;
        }
        //Add line to current editor rows
        insertRow(editorState.numrows, line, linelen);
    }
    free(line);
    fclose(fp);
    editorState.modified = 0;
}

char *rows_to_string(int *buflen) {
    int totlen = 0;

    for (int i = 0; i < editorState.numrows; i++) {
        totlen += editorState.rows[i].length + 1;
    }

    *buflen = totlen;

    char *buf = malloc(totlen);
    char *p = buf;

    for (int i = 0; i < editorState.numrows; i++) {
        memcpy(p, editorState.rows[i].chars, editorState.rows[i].length);
        p += editorState.rows[i].length;

        *p = '\n';
        p++;
    }

    return buf;
}

int file_save() {
    //Check if a file is opened
    if (editorState.filepath == NULL) {
        set_file(prompt("Save as: %s", NULL));

        //User aborted file naming process by pressing ESC
        if (editorState.filepath == NULL) {
            setStatusMessage(0, "Save aborted!");
            return 0;
        }
        determineSyntaxHighlight();
    }

    int len;
    char *buf = rows_to_string(&len);

    int fd = open(editorState.filepath, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);

                editorState.modified = 0;
                setStatusMessage(0, "%d bytes written to disk!", len);

                determineSyntaxHighlight();
                return 1;
            }
        }
        close(fd);
    }
    free(buf);
    setStatusMessage(0, "Error while trying to save: %s", strerror(errno));
    return 0;
}

void set_file(const char *filepath) {
    if (filepath == NULL) return;
    int is_path = (strchr(filepath, '/') != NULL);

    editorState.filepath = (char*) filepath;

    //When the filepath is not a path (does not contain any slashes) we can set the filename to the same value
    if (!is_path) {
        editorState.filename = (char*) filepath;
    } else {
        //When the filepath is actually a path we need to extract the filename
        char *last_slash = strrchr(filepath, '/');
        size_t filepath_len = strlen(filepath);
        //Directly include a place for NULL in the length
        size_t filename_len = filepath_len - (last_slash - filepath);

        editorState.filename = (char*) malloc(filename_len);
        memcpy(editorState.filename, last_slash + 1, filename_len);
    }
}
