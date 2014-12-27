#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algolib/include/btree.h>

int main(int argc, char *argv[])
{
	struct btree *t = btree_create(malloc, free, memset, memcmp, memcpy);
	if (!t) {
		printf("cant create tree\n");
		exit(-1);
	}
	printf("t=%p, root=%p sz=%zu\n", t, t->root, sizeof(*t->root));
	btree_delete(t);
	return 0;
}
