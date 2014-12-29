#pragma once

#include "types.h"

#define BTREE_NODE_KEYS 2047

#pragma pack(push, 1)

struct btree_key {
	u8 bytes[16];
};

struct btree_value {
	union {
		u64 off;
		void *value;
	};
};

struct btree_link {
	union {
		u64 off;
		struct btree_node *addr;
	};
};

#define BTREE_NODE_PAD 16

struct btree_node {
	struct btree_key	keys[BTREE_NODE_KEYS];
	struct btree_value	values[BTREE_NODE_KEYS];
	struct btree_link	childs[BTREE_NODE_KEYS+1];
	u32			nr_keys;
	u32			max_nr_keys;
	u8			pad[BTREE_NODE_PAD];
};

struct btree {
	struct btree_node	*root;
};

#pragma pack(pop)


_Static_assert(sizeof(struct btree_node) == 65536, "size is not correct");

extern struct btree *btree_create(void);

extern void btree_delete(struct btree *tree);

extern int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value);

extern int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value *value);

extern int btree_delete_key(struct btree *tree,
	struct btree_key *key);

