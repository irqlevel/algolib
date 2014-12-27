#pragma once

#include "types.h"

typedef void *	(*al_malloc_t)(size_t size);
typedef void 	(*al_free_t)(void *ptr);

typedef void *	(*al_memset_t)(void *ptr, int value, size_t num);
typedef int	(*al_memcmp_t)(const void *ptr1, const void *ptr2, size_t num);
typedef void *	(*al_memcpy_t)(void *dst, const void *src, size_t num);

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

#define BTREE_NODE_PAD 24

struct btree_node {
	struct btree_key	keys[BTREE_NODE_KEYS];
	struct btree_value	values[BTREE_NODE_KEYS];
	struct btree_link	childs[BTREE_NODE_KEYS+1];
	u8			pad[BTREE_NODE_PAD];
};

struct btree {
	al_malloc_t		malloc;
	al_free_t		free;
	al_memset_t		memset;
	al_memcmp_t		memcmp;
	al_memcpy_t		memcpy;
	struct btree_node	*root;
};

#pragma pack(pop)


_Static_assert(sizeof(struct btree_node) == 65536, "size is not correct");

extern struct btree *btree_create(al_malloc_t malloc,
		al_free_t free,
		al_memset_t memset,
		al_memcmp_t memcmp,
		al_memcpy_t memcpy);

extern void btree_delete(struct btree *tree);

