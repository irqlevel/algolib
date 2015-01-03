#pragma once

#include "types.h"

#define BTREE_T 1024

#pragma pack(push, 1)

struct btree_key {
	u8 bytes[16];
};

struct btree_value {
	union {
		u64 off;
		void *addr;
		u64 value;
	};
};

struct btree_link {
	union {
		u64 off;
		struct btree_node *addr;
	};
};

#define BTREE_NODE_PAD 8

struct btree_node {
	struct btree_key	keys[2*BTREE_T-1];
	struct btree_value	values[2*BTREE_T-1];
	struct btree_link	childs[2*BTREE_T];
	u32			leaf;
	u32			nr_keys;
	u32			max_nr_keys;
	u32			t;
	u8			pad[BTREE_NODE_PAD];
};

struct btree {
	struct btree_node	*root;
	u32			t;
};

struct btree_info {
	u64 nr_keys;
	u64 nr_nodes;
};

#pragma pack(pop)


_Static_assert(sizeof(struct btree_node) == 65536, "size is not correct");

extern struct btree *btree_create(void);

extern void btree_delete(struct btree *tree);

extern int btree_insert_key(struct btree *tree, struct btree_key *key,
	struct btree_value *value, int replace);

extern int btree_find_key(struct btree *tree,
	struct btree_key *key,
	struct btree_value **value);

extern int btree_delete_key(struct btree *tree,
	struct btree_key *key);

extern struct btree_key *btree_gen_key();
extern struct btree_value *btree_gen_value();

extern char *btree_key_hex(struct btree_key *key);
extern char *btree_value_hex(struct btree_value *value);
extern void btree_log(struct btree *tree, int llevel);

extern void btree_stats(struct btree *tree, struct btree_info *info);

extern int btree_check(struct btree *tree);

