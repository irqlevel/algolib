#pragma once

#include "types.h"

typedef void * (*al_malloc_t)(size_t size);
typedef void * (*al_memset_t)(void *ptr, int value, size_t num);
typedef void (*al_free_t)(void *ptr);

#define BTREE_T	1365

#pragma pack(push, 1)
struct btree_key {
	u8 bytes[16];
};

struct btree_link {
	union {
		u64 off;
		struct btree_node *addr;	
	};
};

#define BTREE_NODE_PAD_BYTES 24

struct btree_node {
	struct btree_key	keys[2*BTREE_T-1];
	struct btree_link	childs[2*BTREE_T];
	struct btree_link	data;
	u8			pad[BTREE_NODE_PAD_BYTES];
};

_Static_assert(BTREE_NODE_PAD_BYTES < 2*(sizeof(struct btree_key) + sizeof(struct btree_link)), "more childs can be");

struct btree {
	al_malloc_t		malloc;
	al_free_t		free;
	al_memset_t		memset;
	struct btree_node	*root;
};

#pragma pack(pop)

//_Static_assert(sizeof(struct btree_node) == 65536, "size is not correct");

extern struct btree *btree_create(al_malloc_t malloc,
		al_free_t free,
		al_memset_t memset);

extern void btree_delete(struct btree *tree);

