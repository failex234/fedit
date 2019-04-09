#include "fedit.h"
void file_open(char *filename) {
	free(E.filename);
	E.filename = strdup(filename);

	//selectSyntaxHighlight();
	
	FILE *fp = fopen(filename, "r");
	
	//Kill program when file doesn't exist
	if (!fp) {
		die("fopen");
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
		insertRow(E.numrows, line, linelen);
	}
	free(line);
	fclose(fp);
	E.modified = 0;
}

char *rows_to_string(int *buflen) {
	int totlen = 0;
	
	for (int i = 0; i < E.numrows; i++) {
		totlen += E.row[i].size + 1;
	}
	
	*buflen = totlen;
	
	char *buf = malloc(totlen);
	char *p = buf;
	
	for (int i = 0; i < E.numrows; i++) {
		memcpy(p, E.row[i].chars, E.row[i].size);
		p += E.row[i].size;
		
		*p = '\n';
		p++;
	}
	
	return buf;
}

void file_save() {
	//Check if a file is opened
	if (E.filename == NULL) {
		E.filename = prompt("Save as: %s", NULL);

		//User aborted file naming process by pressing ESC
		if (E.filename == NULL) {
			setStatusMessage("Save aborted!");
			return;
		}
		selectSyntaxHighlight();
	}
	
	int len;
	char *buf = rows_to_string(&len);
	
	int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1) {
		if (ftruncate(fd, len) != -1) {
			if (write(fd, buf, len) == len) {
				close(fd);
				free(buf);

				E.modified = 0;
				setStatusMessage("%d bytes written to disk!", len);

				return;
			}
		}
		close(fd);
	}
	free(buf);
	setStatusMessage("Error while trying to save: %s", strerror(errno));
}