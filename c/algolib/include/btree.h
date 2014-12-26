#pragma once

#include "types.h"

typedef void * (*al_malloc_t)(size_t size);
typedef void * (*al_memset_t)(void *ptr, int value, size_t num);
typedef void (*al_free_t)(void *ptr);

#define BTREE_T	1024

struct btree_key {
	char bytes[16];
};

struct btree_link {
	union {
		u64 off;
		struct btree_node *addr;	
	};
};

struct btree_node {
	struct btree_key	keys[2*BTREE_T-1];
	struct btree_link	childs[2*BTREE_T];
	struct btree_link	data;
};


struct btree {
	al_malloc_t		malloc;
	al_free_t		free;
	al_memset_t		memset;
	struct btree_node	*root;
};

extern struct btree *btree_create(al_malloc_t malloc,
		al_free_t free,
		al_memset_t memset);

extern void btree_delete(struct btree *tree);

