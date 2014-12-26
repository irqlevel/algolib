#include <include/btree.h>

static struct btree_node *btree_node_create(struct btree *tree)
{
	struct btree_node *node;

	node = tree->malloc(sizeof(*node));
	if (!node)
		return NULL;
	tree->memset(node, 0, sizeof(*node));
	return node;	
}

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
	
	tree->root = btree_node_create(tree);
	if (!tree->root) {
		goto fail;
	}

	return tree;
fail:
	btree_delete(tree);
	return NULL;
}

void btree_delete(struct btree *tree)
{
	if (tree->root)
		tree->free(tree->root);
	tree->free(tree);
}

