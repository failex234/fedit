#include "fedit.h"
void file_open(char *filename) {
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
		appendRow(line, linelen);
	}
	free(line);
	fclose(fp);
}