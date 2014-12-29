#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algolib/include/algolib.h>

static void prepare(void)
{
	struct al_crt rt;
	rt.malloc = malloc;
	rt.free = free;
	rt.memcmp = memcmp;
	rt.memcpy = memcpy;
	rt.memset = memset;
	al_init(&rt);
}

int main(int argc, char *argv[])
{
	struct btree *t;

	prepare();	

	t = btree_create();
	if (!t) {
		printf("cant create tree\n");
		exit(-1);
	}
	printf("t=%p, root=%p sz=%zu\n", t, t->root, sizeof(*t->root));
	btree_delete(t);
	return 0;
}
