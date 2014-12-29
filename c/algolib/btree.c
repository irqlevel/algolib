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
	node->leaf = 0;
	node->t = (node->max_nr_keys + 1)/2;
	AL_LOG(AL_INF, "node %p created", node);
	return node;	
}

static void btree_node_delete(struct btree_node *node)
{
	al_free(node);
	AL_LOG(AL_INF, "node %p leaf %d deleted", node, node->leaf);
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
	tree->root->leaf = 1;
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
			if (!curr->leaf)
				curr = curr->childs[index].addr;
			else {
				curr = NULL;
				break;
			}
		}
	}

	return curr;
}
/*
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
*/

static void btree_node_copy_kv(struct btree_node *dst, int dst_index,
	struct btree_node *src, int src_index)
{
	btree_copy_key(&dst->keys[dst_index], &src->keys[src_index]);
	btree_copy_value(&dst->values[dst_index], &src->values[src_index]);
}

static int btree_node_split_child(struct btree_node *node,
		int child_index)
{
	struct btree_node *new;
     	struct btree_node *child;
	int i;

	child = node->childs[child_index].addr;
	AL_BUG_ON(!btree_node_is_full(child));
	new = btree_node_create();
	if (!new)
		return -1;

	new->leaf = child->leaf;

	for (i = 0; i < new->t - 1; i++)
		btree_node_copy_kv(new, i, child, i + new->t);

	if (!child->leaf)
		for (i = 0; i < new->t; i++)
			new->childs[i].addr = child->childs[i + new->t].addr;

	child->nr_keys = new->t - 1;
	for (i = node->nr_keys; i >= (child_index + 1); i--)
		node->childs[i + 1].addr = node->childs[i].addr;
	node->childs[child_index + 1].addr = new;

	for (i = node->nr_keys; i >= child_index; i--)
		btree_node_copy_kv(node, i+1, node, i);	
	
	btree_node_copy_kv(node, child_index, child, new->t);
	node->nr_keys++;
	return 0;	
}

static int btree_node_insert_nonfull(struct btree_node *node,
	struct btree_key *key,
	struct btree_value *value)
{
	int i = node->nr_keys;
	if (node->leaf) {
		while (i >= 0 && btree_cmp_key(key, &node->keys[i]) < 0) {
			btree_node_copy_kv(node, i + 1, node, i);
			i--;
		}
		btree_copy_key(&node->keys[i+1], key);
		btree_copy_value(&node->values[i+1], value);
		node->nr_keys++;
		return 0;
	} else {
		struct btree_node *child;
		while (i >=0 && btree_cmp_key(key, &node->keys[i]) < 0)
			i--;
		i++;
		child = node->childs[i].addr;
		if (btree_node_is_full(child)) {
			if (btree_node_split_child(node, i) < 0)
				return -1;
			if (btree_cmp_key(key, &node->keys[i]) > 0)
				i++;		
		}
		return btree_node_insert_nonfull(node->childs[i].addr, key, value);
	}
}

int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value)
{
	struct btree_node *root = tree->root;
	if (btree_key_is_zero(key)) {
		AL_LOG(AL_ERR, "key is zero");
		return -1;
	}

	if (btree_node_is_full(root)) {
		struct btree_node *new;
		new = btree_node_create();
		new->leaf = 0;
		new->nr_keys = 0;
		new->childs[0].addr = root;
		if (btree_node_split_child(new, 0) < 0) {
			btree_node_delete(new);
			return -1;
		}
		if (btree_node_insert_nonfull(new, key, value) < 0) {
			btree_node_delete(new);
			return -1;
		}
		tree->root = new;
		return 0;
	} else {
		return btree_node_insert_nonfull(root, key, value);
	}
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
	return 0;
}
