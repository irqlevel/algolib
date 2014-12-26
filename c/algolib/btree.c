#include <include/algolib.h>

struct btree *btree_create(al_malloc_t malloc,
		al_free_t free,
		al_memset_t memset)
{
	struct btree *tree;

	if (!malloc || !free || !memset)
		return NULL;

	tree = malloc(sizeof(*tree));
	if (!tree)
		return NULL;
	memset(tree, 0, sizeof(*tree));
	tree->malloc = malloc;
	tree->memset = memset;
	tree->free = free;

	return tree;
}
void btree_delete(struct btree *tree)
{
	tree->free(tree);
}

