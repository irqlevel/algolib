#include <include/crt.h>
#include <include/btree.h>

static struct btree_node *btree_node_create(void)
{
	struct btree_node *node;

	node = al_malloc(sizeof(*node));
	if (!node)
		return NULL;
	al_memset(node, 0, sizeof(*node));
	return node;	
}

struct btree *btree_create(void)
{
	struct btree *tree;

	tree = al_malloc(sizeof(*tree));
	if (!tree)
		return NULL;

	al_memset(tree, 0, sizeof(*tree));
	tree->root = btree_node_create();
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
		al_free(tree->root);
	al_free(tree);
}

static int btree_cmp_key(
	struct btree_key *key1,
	struct btree_key *key2)
{
	return al_memcmp(key1, key2, sizeof(*key1));
}
/*
static void btree_copy_key(struct btree *tree,
	struct btree_key *dst,
	struct btree_key *src)
{
	tree->memcpy(dst, src, sizeof(*dst));
}
*/

static void btree_copy_value(
	struct btree_value *dst,
	struct btree_value *src)
{
	al_memcpy(dst, src, sizeof(*dst));
}

static struct btree_key btree_zero_key = {{0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0}};

static int btree_key_is_zero(struct btree_key *key)
{
	return (0 == btree_cmp_key(key, &btree_zero_key)) ? 1 : 0;
}

static int btree_node_key_next(struct btree_node *node,
		int prev_index)
{
	int nkeys = sizeof(node->keys)/sizeof(node->keys[0]);
	int i;
	
	if (prev_index < -1 || prev_index >= nkeys)
		return -1;

	for (i = prev_index + 1; i < nkeys; i++) {
		if (btree_key_is_zero(&node->keys[i]))
			continue;
		else
			break;
	}

	return (i == nkeys) ? -1 : i;
}

static int btree_node_key_index(
	struct btree_node *node,
	struct btree_key *key,
	int *index)
{
	int curr, first, last;
	int cmp;	

	first = btree_node_key_next(node, -1);
	if (first < 0)
		return -1;

	cmp = btree_cmp_key(key, &node->keys[first]);
	if (cmp == 0) {
		*index = 0;
		return 0;
	}

	if (cmp < 0) {
		*index = 0;
		return 1;
	}	

	curr = first;
	while (1) {
		last = curr;
		curr = btree_node_key_next(node, curr);
		if (curr < 0)
			break;
		cmp = btree_cmp_key(key, &node->keys[curr]);
		if (cmp == 0) {
			*index = curr;
			return 0;
		}

		if (cmp < 0) {
			*index = curr;
			return 1;
		}
	}

	*index = last + 1;		
 	return 1;
}

static struct btree_node *btree_find_node_key(struct btree *tree,
	struct btree_key *key,
	int *pindex)
{
	struct btree_node *curr = tree->root;
	int index;

	while (curr != NULL) {
		int res = btree_node_key_index(curr, key, &index);
		if (res < 0)
			return NULL;
		else if (0 == res) {
			*pindex = index;
			break;
		} else {
			curr = curr->childs[index].addr;
		}
	}

	return curr;
}

static int btree_insert_node_key(struct btree *tree,
	struct btree_node *node, struct btree_node *parent, 
	struct btree_key *key,
	struct btree_value *value)
{
	return -1;
}

int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *curr = tree->root, *prev = NULL, *prevprev = NULL;
	int index;

	if (btree_key_is_zero(key))
		return -1;

	while (curr != NULL) {
		int res = btree_node_key_index(curr, key, &index);
		if (res < 0) {
			return -1;
		} else if (res == 0) {
			btree_copy_value(&curr->values[index], value);
			return 1;
		} else {
			prevprev = prev;
			prev = curr;
			curr = curr->childs[index].addr;
		}
	}

	return btree_insert_node_key(tree, prev, prevprev, key, value);
}

int btree_delete_key(struct btree *tree, struct btree_key *key)
{
	if (btree_key_is_zero(key))
		return -1;

	return -1;
}

int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *node;
	int index;

	if (btree_key_is_zero(key))
		return -1;

	node = btree_find_node_key(tree, key, &index);
	if (node == NULL)
		return -1;

	btree_copy_value(value, &node->values[index]);
	return 1;
}
