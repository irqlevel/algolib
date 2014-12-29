#pragma once

#include "types.h"

typedef void *	(*al_malloc_t)(size_t size);
typedef void 	(*al_free_t)(void *ptr);
typedef void *	(*al_memset_t)(void *ptr, int value, size_t num);
typedef int	(*al_memcmp_t)(const void *ptr1, const void *ptr2, size_t num);
typedef void *	(*al_memcpy_t)(void *dst, const void *src, size_t num);

struct al_crt {
	al_malloc_t	malloc;
	al_free_t	free;
	al_memset_t	memset;
	al_memcpy_t	memcpy;
	al_memcmp_t	memcmp;
};

extern void *al_malloc(size_t size);
extern void al_free(void *ptr);
extern void *al_memset(void *ptr, int value, size_t num);
extern int al_memcmp(const void *ptr1, const void *ptr2, size_t num);
extern void *al_memcpy(void *dst, const void *src, size_t num);

extern void al_init(struct al_crt *ctx);

