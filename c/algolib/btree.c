#include <include/crt.h>
#include <include/btree.h>

static struct btree_node *btree_node_create(void)
{
	struct btree_node *node;

	node = al_malloc(sizeof(*node));
	if (!node) {
		AL_LOG(AL_ERR, "no memory");
		return NULL;
	}
	al_memset(node, 0, sizeof(*node));
	node->max_nr_keys = ARRAY_SIZE(node->keys);
	AL_LOG(AL_INF, "node %p created", node);
	return node;	
}

static void btree_node_delete(struct btree_node *node)
{
	al_free(node);
	AL_LOG(AL_INF, "node %p deleted", node);
}

struct btree *btree_create(void)
{
	struct btree *tree;

	tree = al_malloc(sizeof(*tree));
	if (!tree) {
		AL_LOG(AL_ERR, "no memory");
		return NULL;
	}

	al_memset(tree, 0, sizeof(*tree));
	tree->root = btree_node_create();
	if (!tree->root) {
		goto fail;
	}

	AL_LOG(AL_INF, "tree %p created", tree);
	return tree;
fail:
	btree_delete(tree);
	return NULL;
}

void btree_delete(struct btree *tree)
{
	if (tree->root)
		btree_node_delete(tree->root);
	al_free(tree);
	AL_LOG(AL_INF, "tree %p deleted", tree);
}

static int btree_cmp_key(
	struct btree_key *key1,
	struct btree_key *key2)
{
	return al_memcmp(key1, key2, sizeof(*key1));
}

static void btree_copy_key(
	struct btree_key *dst,
	struct btree_key *src)
{
	al_memcpy(dst, src, sizeof(*dst));
}

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

static int btree_node_is_full(struct btree_node *node)
{
	return (node->max_nr_keys == node->nr_keys) ? 1 : 0;
}

static int btree_node_key_index(
	struct btree_node *node,
	struct btree_key *key,
	int *match)
{
	int i;
	int cmp;

	for (i = 0; i < node->nr_keys; i++) {
		cmp = btree_cmp_key(key, &node->keys[i]);
		if (cmp < 0) {
			*match = 0;
			return i;
		} else if (cmp == 0) {
			*match = 1;
			return i;
		}
	}
	*match = 0;
	return i;
}

static struct btree_node *btree_find_node_key(struct btree *tree,
	struct btree_key *key,
	int *pindex)
{
	struct btree_node *curr = tree->root;
	int index;
	int match;

	while (curr != NULL) {
		index = btree_node_key_index(curr, key, &match);
		if (match) {
			*pindex = index;
			break;
		} else {
			curr = curr->childs[index].addr;
		}
	}

	return curr;
}

static void btree_node_swap_key(struct btree_node *node,
		int i, int j)
{
	AL_BUG_ON(i < 0 || j < 0 || i >= node->max_nr_keys ||
		       j >= node->max_nr_keys);

	if (i != j) {
		struct btree_key tmp;
		btree_copy_key(&tmp, &node->keys[i]);
		btree_copy_key(&node->keys[i], &node->keys[j]);
		btree_copy_key(&node->keys[j], &tmp);		
	}
}

static void btree_node_swap_value(struct btree_node *node,
		int i, int j)
{
	AL_BUG_ON(i < 0 || j < 0 || i >= node->max_nr_keys ||
		       j >= node->max_nr_keys);

	if (i != j) {
		struct btree_value tmp;
		btree_copy_value(&tmp, &node->values[i]);
		btree_copy_value(&node->values[i], &node->values[j]);
		btree_copy_value(&node->values[j], &tmp);
	}
}

static int btree_node_place_key(struct btree_node *node,
		struct btree_key *key, struct btree_value *value)
{
	int i;
	int pos = node->nr_keys;
	
	AL_BUG_ON(node->nr_keys >= node->max_nr_keys);

	btree_copy_key(&node->keys[pos], key);
	btree_copy_value(&node->values[pos], value);
	
	for (i = 0; i < node->nr_keys; i++) {
		if (btree_cmp_key(&node->keys[pos], &node->keys[i]) < 0) {
			btree_node_swap_key(node, pos, i);
			btree_node_swap_value(node, pos, i);
		}
	}
	node->nr_keys++;

	return 1;
}

static int btree_insert_node_key(struct btree *tree,
	struct btree_node *node, struct btree_key *key,
	struct btree_value *value)
{
	if (!btree_node_is_full(node))
		return btree_node_place_key(node, key, value);
	AL_LOG(AL_ERR, "not implemented");
	return -1;
}

int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *curr = tree->root, *prev = NULL;
	int index;
	int match;

	if (btree_key_is_zero(key)) {
		AL_LOG(AL_ERR, "key is zero");
		return -1;
	}

	AL_BUG_ON(!curr);
	while (curr != NULL) {
		index = btree_node_key_index(curr, key, &match);
		if (match) {
			btree_copy_value(&curr->values[index], value);
			return 1;
		} else {
			prev = curr;
			curr = curr->childs[index].addr;
		}
	}

	return btree_insert_node_key(tree, prev, key, value);
}

int btree_delete_key(struct btree *tree, struct btree_key *key)
{
	if (btree_key_is_zero(key))
		return -1;

	AL_LOG(AL_ERR, "not implemented");
	return -1;
}

int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *node;
	int index;

	if (btree_key_is_zero(key)) {
		AL_LOG(AL_ERR, "key is zero");
		return -1;
	}

	node = btree_find_node_key(tree, key, &index);
	if (node == NULL)
		return -1;

	btree_copy_value(value, &node->values[index]);
	return 1;
}
