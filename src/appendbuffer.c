#include "fedit.h"

void abAppend(struct screen_buffer *ab, const char *s, int len) {
    //Reallocate memory for the new string that should get appended
    char *new = realloc(ab->str_buf, ab->len + len);
    if (new == NULL) return;

    //Copy the new string behind the "old" string
    memcpy(&new[ab->len], s, len);

    //Update the struct
    ab->str_buf = new;
    ab->len += len;
}
void abFree(struct screen_buffer *ab) {
    free(ab->str_buf);
}