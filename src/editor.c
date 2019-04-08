#include "fedit.h"
void rowInsertChar(erow *row, int at, int c) {
	if (at < 0 || at > row->size) {
		at = row->size;
	}
}