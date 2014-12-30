#include <include/crt.h>
#include <include/char.h>
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

	AL_BUG_ON((node->max_nr_keys + 1) & 1);
	node->t = (node->max_nr_keys + 1)/2;

	AL_LOG(AL_DBG, "node %p created", node);
	return node;	
}

static void btree_node_delete(struct btree_node *node)
{
	struct btree_node *child;
	int i;

	if (node->nr_keys) {
		for (i = 0; i < node->nr_keys + 1; i++) {
			child = node->childs[i].addr;
			if (child != NULL)
				btree_node_delete(child);
		}
	}
	AL_LOG(AL_DBG, "node %p leaf %d deleted", node, node->leaf);
	al_free(node);
}

struct btree_key *btree_gen_key()
{
	struct btree_key *key;
	key = al_malloc(sizeof(*key));
	if (al_random_buf(key, sizeof(*key))) {
		al_free(key);
		return NULL;
	}
	return key;
}

struct btree_value *btree_gen_value()
{
	struct btree_value *value;
	value = al_malloc(sizeof(*value));
	if (al_random_buf(value, sizeof(*value))) {
		al_free(value);
		return NULL;
	}
	return value;
}

char *btree_key_hex(struct btree_key *key)
{
	return bytes_hex((char *)key, sizeof(*key));
}

char *btree_value_hex(struct btree_value *value)
{
	return bytes_hex((char *)value, sizeof(*value));
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
	AL_LOG(AL_DBG, "tree %p created", tree);
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
	AL_LOG(AL_DBG, "tree %p deleted", tree);
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

static void btree_node_copy_kv(struct btree_node *dst, int dst_index,
	struct btree_node *src, int src_index)
{
	btree_copy_key(&dst->keys[dst_index], &src->keys[src_index]);
	btree_copy_value(&dst->values[dst_index], &src->values[src_index]);
}

static void btree_node_split_child(struct btree_node *node,
		int index, struct btree_node *new)
{
	struct btree_node *child;
	int i;

	AL_BUG_ON(index < 0 || index > node->nr_keys);
	child = node->childs[index].addr;
	AL_BUG_ON(!child || !btree_node_is_full(child));	
	AL_BUG_ON(!new);

	new->leaf = child->leaf;
	/* copy T-1 keys from child to new */
	for (i = 0; i < new->t - 1; i++)
		btree_node_copy_kv(new, i, child, i + new->t);
	new->nr_keys = new->t - 1;

	/* copy T childs from child to new */
	if (!child->leaf)
		for (i = 0; i < new->t; i++)
			new->childs[i].addr = child->childs[i + new->t].addr;

	/* shift node childs to the right by one */
	child->nr_keys = new->t - 1;
	for (i = (node->nr_keys); i >= (index + 1); i--)
		node->childs[i + 1].addr = node->childs[i].addr;

	/* setup node new child */
	node->childs[index + 1].addr = new;

	/* shift node keys to the right by one */
	for (i = (node->nr_keys - 1); i >= index; i--)
		btree_node_copy_kv(node, i+1, node, i);	

	/* move mid key from child to node */
	btree_node_copy_kv(node, index, child, new->t - 1);
	node->nr_keys++;
	AL_LOG(AL_DBG, "Splitted node %p nr_keys=%d", node, node->nr_keys);
}

static int btree_node_has_key(struct btree_node *node, struct btree_key *key)
{
	int i;
	for (i = 0; i < node->nr_keys; i++) {
		if (0 == btree_cmp_key(&node->keys[i], key))
			return i;
	}
	return -1;
}

static int btree_node_insert_nonfull(struct btree_node *node,
	struct btree_key *key,
	struct btree_value *value,
	int replace)
{
	int i, j;
	while (1) {
		i = node->nr_keys - 1;
		/* if key exists replace value */
		j = btree_node_has_key(node, key);
		if (j >= 0) {
			if (replace) {
				btree_copy_value(
						&node->values[j],
						value);
				return 0;
			} else {
				return -1;
			}
		}

		if (node->leaf) {
			/* key doesnt exists so place key value in sorted
			 * order
			 */
			while (i >= 0 && btree_cmp_key(key,
						&node->keys[i]) < 0) {
				btree_node_copy_kv(node, i + 1, node, i);
				i--;
			}
			btree_copy_key(&node->keys[i+1], key);
			btree_copy_value(&node->values[i+1], value);
			node->nr_keys++;
			AL_LOG(AL_DBG, "inserted key into node=%p nr_keys=%d", node, node->nr_keys);
			return 0;
		} else {
			struct btree_node *child;
			while (i >=0 && btree_cmp_key(key, &node->keys[i]) < 0)
				i--;
			i++;
			child = node->childs[i].addr;
			if (btree_node_is_full(child)) {
				struct btree_node *new;
				new = btree_node_create();
				if (!new)
					return -1;
				btree_node_split_child(node, i, new);
				if (btree_cmp_key(key, &node->keys[i]) > 0)
					i++;		
			}
			node = node->childs[i].addr;
		}
	}
}

int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value,
	int replace)
{
	struct btree_node *root = tree->root;
	if (btree_key_is_zero(key)) {
		AL_LOG(AL_ERR, "key is zero");
		return -1;
	}

	if (btree_node_is_full(root)) {
		int rc;
		struct btree_node *new, *new2;
		new = btree_node_create();
		if (new == NULL)
			return -1;
		new2 = btree_node_create();
		if (new2 == NULL) {
			btree_node_delete(new2);
			return -1;
		}

		new->leaf = 0;
		new->nr_keys = 0;
		new->childs[0].addr = root;
		btree_node_split_child(new, 0, new2);	

		if (btree_node_insert_nonfull(new, key, value, replace) < 0)
			rc = -1;
		else
			rc = 0;

		tree->root = new;
		return rc;
	} else {
		return btree_node_insert_nonfull(root, key, value, replace);
	}
}

static struct btree_node *btree_find_node_key(struct btree_node *node,
		struct btree_key *key, int *pindex)
{
	int i;

	while (1) {
		if (node->nr_keys == 0)
			return NULL;
		i = 0;
		while (i < node->nr_keys &&
				btree_cmp_key(key, &node->keys[i]) > 0)
			i++;

		if (i < node->nr_keys &&
				btree_cmp_key(key, &node->keys[i]) == 0) {
			*pindex = i;
			return node;
		} else if (node->leaf)
			return NULL;
		else
			node = node->childs[i].addr;
	}
}

int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value **pvalue)
{
	struct btree_node *node;
	struct btree_value *value;
	int index;

	if (btree_key_is_zero(key)) {
		AL_LOG(AL_ERR, "key is zero");
		return -1;
	}

	node = btree_find_node_key(tree->root, key, &index);
	if (node == NULL)
		return -1;

	value = al_malloc(sizeof(*value));
	btree_copy_value(value, &node->values[index]);
	*pvalue = value;

	return 0;
}

static int btree_node_leaf_delete_key(struct btree_node *node,
		int index)
{
	int i;

	AL_BUG_ON(!node->leaf);
	AL_BUG_ON(index < 0 || index >= node->nr_keys);

	for (i = (index + 1); i < node->nr_keys; i++) {
		btree_node_copy_kv(node, i-1, node, i);	
	}
	node->nr_keys--;
	return 0;
}

static int btree_node_delete_key(struct btree_node *node, struct btree_key *key)
{
	int i;
	if ((i = btree_node_has_key(node, key)) >= 0) {
		if (node->leaf)
			return btree_node_leaf_delete_key(node, i);
	}
	AL_LOG(AL_ERR, "not implemented");
	return -1;
}

int btree_delete_key(struct btree *tree, struct btree_key *key)
{
	if (btree_key_is_zero(key))
		return -1;

	return btree_node_delete_key(tree->root, key);
}


static void btree_log_node(struct btree_node *node, u32 height, int llevel)
{
	struct btree_node *child;
	int i;

	AL_LOG(llevel, "node %p nr_keys %d leaf %d height %u",
			node, node->nr_keys, node->leaf, height);

	if (node->nr_keys) {
		for (i = 0; i < node->nr_keys + 1; i++) {
			child = node->childs[i].addr;
			if (child != NULL)
				btree_log_node(child, height+1, llevel);
		}
	}
}

static void btree_node_stats(struct btree_node *node, u64 *nr_keys,
	u64 *nr_nodes)
{
	struct btree_node *child;
	int i;

	if (node->nr_keys) {
		for (i = 0; i < node->nr_keys + 1; i++) {
			child = node->childs[i].addr;
			if (child != NULL)
				btree_node_stats(child, nr_keys, nr_nodes);
		}
	}
	*nr_nodes+= 1;
	*nr_keys+= node->nr_keys;
}

void btree_stats(struct btree *tree, u64 *nr_keys, u64 *nr_nodes)
{
	*nr_keys = 0;
	*nr_nodes = 0;
	btree_node_stats(tree->root, nr_keys, nr_nodes);
}

void btree_log(struct btree *tree, int llevel)
{
	btree_log_node(tree->root, 1, llevel);
}
