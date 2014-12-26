#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algolib/include/btree.h>

int main(int argc, char *argv[])
{
	struct btree *t = btree_create(malloc, free, memset);
	if (!t) {
		printf("cant create tree\n");
		exit(-1);
	}
	printf("t=%p\n", t);
	btree_delete(t);
	return 0;
}
