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
		al_memset_t memset,
		al_memcmp_t memcmp,
		al_memcpy_t memcpy)
{
	struct btree *tree;

	if (!malloc || !free || !memset || !memcmp)
		return NULL;

	tree = malloc(sizeof(*tree));
	if (!tree)
		return NULL;

	memset(tree, 0, sizeof(*tree));
	tree->malloc = malloc;
	tree->free = free;
	tree->memset = memset;
	tree->memcmp = memcmp;

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

static int btree_cmp_key(struct btree *tree,
	struct btree_key *key1,
	struct btree_key *key2)
{
	return tree->memcmp(key1, key2, sizeof(*key1));
}
/*
static void btree_copy_key(struct btree *tree,
	struct btree_key *dst,
	struct btree_key *src)
{
	tree->memcpy(dst, src, sizeof(*dst));
}
*/

static void btree_copy_value(struct btree *tree,
	struct btree_value *dst,
	struct btree_value *src)
{
	tree->memcpy(dst, src, sizeof(*dst));
}


static struct btree_node *btree_find_child(struct btree *btree,
	struct btree_node *curr, struct btree_key *key)
{
	return NULL;
}

static struct btree_node *btree_find_node(struct btree *tree, struct btree_key *key)
{
	return btree_find_child(tree, tree->root, key);
}

struct btree_value *btree_find_value(struct btree *tree, struct btree_key *key)
{
	struct btree_node *node;
	struct btree_value *value;
	node = btree_find_node(tree, key);
	if (!node)
		return NULL;
	if (0 != btree_cmp_key(tree, key, &node->key))
		return NULL;
	value = tree->malloc(sizeof(*value));
	btree_copy_value(tree, value, &node->value);
	return value;
}
