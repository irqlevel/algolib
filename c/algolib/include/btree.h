#pragma once


typedef void * (*al_malloc_t)(size_t size);
typedef void * (*al_memset_t)(void *ptr, int value, size_t num);
typedef void (*al_free_t)(void *ptr);

struct btree {
	al_malloc_t	malloc;
	al_free_t	free;
	al_memset_t	memset;
};

extern struct btree *btree_create(al_malloc_t malloc,
		al_free_t free,
		al_memset_t memset);

extern void btree_delete(struct btree *tree);

