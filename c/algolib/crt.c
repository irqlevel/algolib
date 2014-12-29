#include <include/crt.h>

static struct al_crt g_rt;

void al_init(struct al_crt *rt)
{
	g_rt.malloc = rt->malloc;
	g_rt.free = rt->free;
	g_rt.memset = rt->memset;
	g_rt.memcpy = rt->memcpy;
	g_rt.memcmp = rt->memcmp;
}

static struct al_crt *get_rt() {
	return &g_rt;
}

void *al_malloc(size_t size)
{
	return get_rt()->malloc(size);

}

void al_free(void *ptr)
{
	get_rt()->free(ptr);
}

void *al_memset(void *ptr, int value, size_t num)
{
	return get_rt()->memset(ptr, value, num);
}

int al_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
	return get_rt()->memcmp(ptr1, ptr2, num);
}

void *al_memcpy(void *dst, const void *src, size_t num)
{
	return get_rt()->memcpy(dst, src, num);
}
