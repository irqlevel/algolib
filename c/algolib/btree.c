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

static int btree_node_key_index(struct btree *tree,
	struct btree_node *node,
	struct btree_key *key,
	int *index)
{
	int i;
	int num_keys = sizeof(node->keys)/sizeof(node->keys[0]);
	int cmp;	
 	
	cmp = btree_cmp_key(tree, key, &node->keys[0]);
	if (cmp == 0) {
		*index = 0;
		return 0;
	}

	if (cmp < 0) {
		*index = 0;
		return 1;
	}	

	for (i = 1; i <	num_keys;
		i++) {
		cmp = btree_cmp_key(tree, key, &node->keys[i]);
		if (cmp == 0) {
			*index = i;
			return 0;
		}

		if (cmp < 0) {
			*index = i;
			return 1;
		}
	}

	*index = num_keys;		
 	return 1;
}

static struct btree_node *btree_find_node_key(struct btree *tree,
	struct btree_key *key,
	int *pindex)
{
	struct btree_node *curr = tree->root;
	int index;

	while (curr != NULL) {
		if (0 == btree_node_key_index(tree, curr, key, &index)) {
			*pindex = index;
			break;
		}
		curr = curr->childs[index].addr;
	}

	return curr;
}

int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *node;
	int index;

	node = btree_find_node_key(tree, key, &index);
	if (node == NULL)
		return -1;

	btree_copy_value(tree, value, &node->values[index]);
	return 0;
}

static int btree_insert_node_key(struct btree *tree,
	struct btree_node *node, struct btree_key *key,
	struct btree_value *value)
{
	return -1;
}

int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *curr = tree->root, *prev = NULL;
	int index;

	while (curr != NULL) {
		if (0 == btree_node_key_index(tree, curr, key, &index)) {
			btree_copy_value(tree, &curr->values[index], value);
			return 0;
		}
		prev = curr;
		curr = curr->childs[index].addr;
	}

	return btree_insert_node_key(tree, prev, key, value);
}

int btree_delete_key(struct btree *tree, struct btree_key *key)
{
	return -1;
}
