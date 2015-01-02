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

static void __btree_node_free(struct btree_node *node)
{
	AL_LOG(AL_DBG, "node %p leaf %d nr_keys %d",
		node, node->leaf, node->nr_keys);
	al_free(node);
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
	AL_LOG(AL_DBG, "node %p leaf %d nr_keys %d",
		node, node->leaf, node->nr_keys);
	__btree_node_free(node);
}

struct btree_key *btree_gen_key()
{
	struct btree_key *key;
	key = al_malloc(sizeof(*key));
	if (!key)
		return NULL;

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
	if (!value)
		return NULL;

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

static void btree_node_zero_kv(struct btree_node *dst, int dst_index)
{
	al_memset(&dst->keys[dst_index], 0, sizeof(struct btree_key));
	al_memset(&dst->values[dst_index], 0, sizeof(struct btree_value));
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
			AL_LOG(AL_DBG, "inserted key into node=%p nr_keys=%d",
					node, node->nr_keys);
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
			btree_node_delete(new);
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

static int 
btree_node_find_key_index(struct btree_node *node,
	struct btree_key *key)
{
	int i = 0;
	while (i < node->nr_keys && btree_cmp_key(key, &node->keys[i]) > 0)
		i++;
	return i;
}
 
static struct btree_node *btree_node_find_key(struct btree_node *node,
		struct btree_key *key, int *pindex)
{
	int i;

	while (1) {
		if (node->nr_keys == 0)
			return NULL;

		i = btree_node_find_key_index(node, key);
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

	node = btree_node_find_key(tree->root, key, &index);
	if (node == NULL)
		return -1;

	value = al_malloc(sizeof(*value));
	if (!value)
		return -1;

	btree_copy_value(value, &node->values[index]);
	*pvalue = value;

	return 0;
}

static void __btree_node_delete_child_index(struct btree_node *node,
		int index)
{
	int i;
	
	/* do shift to the left by one */
	for (i = (index + 1); i < node->nr_keys + 1; i++) {
		node->childs[i-1].addr = node->childs[i].addr;
	}
	/* zero last slot */
	node->childs[i-1].addr = NULL;	
}

static void __btree_node_delete_key_index(struct btree_node *node,
		int index)
{
	int i;

	AL_BUG_ON(node->nr_keys < 1);
	/* do shift to the left by one */
	for (i = (index + 1); i < node->nr_keys; i++) {
		btree_node_copy_kv(node, i-1, node, i);	
	}
	/* zero last slot */
	btree_node_zero_kv(node, i-1);
}


static void btree_node_leaf_delete_key(struct btree_node *node,
		struct btree_key *key)
{
	int index;
	AL_BUG_ON(!node->leaf);
	index = btree_node_has_key(node, key);
	AL_BUG_ON(index < 0 || index >= node->nr_keys);
	__btree_node_delete_key_index(node, index);
	node->nr_keys--;
}


static struct btree_node *
btree_node_find_left_most(struct btree_node *node, int *pindex)
{
	struct btree_node *curr = node;
	while (1) {
		if (curr->leaf) {
			*pindex = curr->nr_keys - 1;
			return curr;
		}
		curr = curr->childs[curr->nr_keys].addr;
	}
}

static struct btree_node *
btree_node_find_right_most(struct btree_node *node, int *pindex)
{
	struct btree_node *curr = node;
	while (1) {
		if (curr->leaf) {
			*pindex = 0;
			return curr;
		}
		curr = curr->childs[0].addr;
	}
}

static void btree_node_merge(struct btree_node *dst,
	struct btree_node *src, struct btree_key *key,
	struct btree_value *value)
{
	int i, pos, cpos;

	/* copy mid key and value */
	btree_copy_key(&dst->keys[dst->nr_keys], key);
	btree_copy_value(&dst->values[dst->nr_keys], value);

	pos = dst->nr_keys + 1;
	cpos = pos + 1;
	for (i = 0; i < src->nr_keys; i++, pos++, cpos++) {
		/* copy key-values */
		btree_node_copy_kv(dst, pos, src, i);
		/* copy childs */
		dst->childs[cpos].addr = src->childs[i].addr;
	}
	/* copy last child */
	dst->childs[cpos].addr = src->childs[i].addr;
	/* update keys num */
	dst->nr_keys+= 1 + src->nr_keys;
}

static int btree_node_delete_key(struct btree_node *node,
		struct btree_key *key);

static int btree_node_internal_delete_key(struct btree_node *node,
		struct btree_key *key)
{
	struct btree_node *pre_child = NULL, *suc_child = NULL;
	int index;

	index = btree_node_has_key(node, key);
	AL_BUG_ON(index < 0);

	pre_child = node->childs[index].addr;
	suc_child = node->childs[index+1].addr;

	if (pre_child->nr_keys >= pre_child->t) {
		struct btree_node *pre;
		int pre_index;

		pre = btree_node_find_left_most(pre_child, &pre_index);
		btree_node_copy_kv(node, index, pre, pre_index);
		return btree_node_delete_key(pre, &pre->keys[pre_index]);
	} else if (suc_child->nr_keys >= suc_child->t) {
		struct btree_node *suc;
		int suc_index;

		suc = btree_node_find_right_most(suc_child, &suc_index);
		btree_node_copy_kv(node, index, suc, suc_index);
		return btree_node_delete_key(suc, &suc->keys[suc_index]);
	} else {
		/* merge key and all of suc_child into pre_child */
		/* node loses key and pointer to suc_child */
		int key_index = pre_child->nr_keys;
		
		btree_node_merge(pre_child, suc_child, &node->keys[index],
			&node->values[index]);
		/* delete key from node */
		__btree_node_delete_key_index(node, index);
		__btree_node_delete_child_index(node, index+1);
		node->nr_keys--;
		
		/* delete suc_child */
		__btree_node_free(suc_child);

		/* finally delete key from pre_child */
		return btree_node_delete_key(pre_child,
			&pre_child->keys[key_index]);
	}
}

static int btree_node_child_delete_key(struct btree_node *node,
		struct btree_key *key)
{
	int i;
	struct btree_node *child;

	AL_BUG_ON(btree_node_has_key(node, key) >= 0);
	i = btree_node_find_key_index(node, key);
	child = node->childs[i].addr;
	if (child == NULL) {
		AL_LOG(AL_ERR, "child not found");
		return -1;
	}

	if (child->nr_keys < child->t) {
		struct btree_node *left = (i > 0) ?
			node->childs[i-1].addr : NULL;
		struct btree_node *right = (i <= node->nr_keys) ?
			node->childs[i+1].addr : NULL;
		
		if ((left && left->nr_keys >= left->t) ||
			(right && right->nr_keys >= right->t)) {
			/* give child an extra key by moving a key from node 
			* down into child, moving a key from child's 
			* immediate left or right sibling up into node,
 			* and moving the appropriate child pointer from the 
 			* sibling into child
 			*/
			AL_LOG(AL_ERR, "not implemented");
			return -1;	
		} else if (left && right && left->nr_keys < left->t &&
			right->nr_keys < right->t) {
			/* merge child with left, which involves
 			* moving a key from node down into the new 
 			* merged node to become the median key for
 			* that node.
 			*/

			/* k0 k1 k2	k0 k2
			* c0 c1 c2 c3  c0 c1c2 c3
			*/

			btree_node_merge(left, child, &node->keys[i-1],
				&node->values[i-1]);
			__btree_node_delete_key_index(node, i-1);
			__btree_node_delete_child_index(node, i);
			__btree_node_free(child);
			child = left;
		}	
	}

	return btree_node_delete_key(child, key);
}

static int btree_node_delete_key(struct btree_node *node,
		struct btree_key *key)
{
	struct btree_key key_copy;
	int i;
	
	btree_copy_key(&key_copy, key);
	if ((i = btree_node_has_key(node, &key_copy)) >= 0) {
		if (node->leaf) {
			btree_node_leaf_delete_key(node, &key_copy);
			return 0;
		} else {
			return btree_node_internal_delete_key(node, &key_copy);	
		}
	} else {
		return btree_node_child_delete_key(node, &key_copy);
}
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

static void btree_node_stats(struct btree_node *node,
	struct btree_info *info)
{
	struct btree_node *child;
	int i;

	if (node->nr_keys) {
		for (i = 0; i < node->nr_keys + 1; i++) {
			child = node->childs[i].addr;
			if (child != NULL)
				btree_node_stats(child, info);
		}
	}
	info->nr_nodes++;
	info->nr_keys += node->nr_keys;
}

void btree_stats(struct btree *tree, struct btree_info *info)
{
	al_memset(info, 0, sizeof(*info));
	btree_node_stats(tree->root, info);
}

void btree_log(struct btree *tree, int llevel)
{
	btree_log_node(tree->root, 1, llevel);
}

static int btree_node_check(struct btree_node *node, int root)
{
	int i;
	int errs = 0;
	struct btree_key *prev_key;

	if (root) {
		if (node->nr_keys > (2*node->t - 1)) {
			AL_LOG(AL_ERR, "node %p contains %d keys",
				node, node->nr_keys);
			errs++;
		}
	} else {
		if (node->nr_keys > (2*node->t - 1)) {
			AL_LOG(AL_ERR, "node %p contains %d keys",
				node->nr_keys);
			errs++;
		}

		if (node->nr_keys < (node->t - 1)) {
			AL_LOG(AL_ERR, "node %p contains %d keys",
				node, node->nr_keys);
			errs++;
		}
	}

	prev_key = NULL;
	for (i = 0 ; i < node->nr_keys; i++) {
		if (btree_key_is_zero(&node->keys[i])) {
			AL_LOG(AL_ERR, "node %p zero key %d found",
				node, i);
			errs++;
		} else {
			if (prev_key && (btree_cmp_key(prev_key,
				&node->keys[i]) >= 0)) {
				AL_LOG(AL_ERR, "node %p key %d not sorted",
					node, i);
				errs++;
			}
			prev_key = &node->keys[i];
		}
		if (!node->leaf) {
			if (!node->childs[i].addr) {
				AL_LOG(AL_ERR, "node %p zero child %d found",
					node, i);
				errs++;
			}
		}
	}

	if (!node->leaf) {
		if (!root || (node->nr_keys > 0)) {
			if (!node->childs[i].addr) {
				AL_LOG(AL_ERR, "node %p zero child %d found",
						node, i);
				errs++;
			}
		}
	}

	for (i = 0; i < node->nr_keys+1; i++) {
		struct btree_node *child;
		child = node->childs[i].addr;
		if (child)
			errs+= btree_node_check(child, 0);
	}
	return errs;
}

int btree_check(struct btree *tree)
{
	return btree_node_check(tree->root, 1);
}
