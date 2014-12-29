#pragma once

#include "types.h"

struct al_time {
	u32 year;
	u32 mon;
	u32 day;
	u32 hour;
	u32 min;
	u32 sec;
	u32 usec;
};


typedef void *	(*al_malloc_t)(size_t size);
typedef void 	(*al_free_t)(void *ptr);
typedef void *	(*al_memset_t)(void *ptr, int value, size_t num);
typedef int	(*al_memcmp_t)(const void *ptr1, const void *ptr2, size_t num);
typedef void *	(*al_memcpy_t)(void *dst, const void *src, size_t num);

typedef int (*al_lprintf_t)(const char *fmt, ...);
typedef void (*al_time_t)(struct al_time *time);
typedef int (*al_vsnprintf_t)(char *s, size_t n, const char *fmt, va_list arg);

typedef char * (*al_strrchr_t)(const char *s, int c);
typedef u32 (*al_getpid_t)(void);

struct al_crt {
	al_malloc_t	malloc;
	al_free_t	free;
	al_memset_t	memset;
	al_memcpy_t	memcpy;
	al_memcmp_t	memcmp;
	al_lprintf_t	lprintf;
	al_time_t	time;
	al_vsnprintf_t	vsnprintf;
	al_strrchr_t	strrchr;
	al_getpid_t	getpid;
};

extern void *al_malloc(size_t size);
extern void al_free(void *ptr);
extern void *al_memset(void *ptr, int value, size_t num);
extern int al_memcmp(const void *ptr1, const void *ptr2, size_t num);
extern void *al_memcpy(void *dst, const void *src, size_t num);

extern void al_log(int level, const char *file, int line, const char *func,
	const char *fmt, ...);

extern void al_init(struct al_crt *ctx);

#define AL_INV 0
#define AL_DBG 1
#define AL_INF 2
#define AL_WRN 3
#define AL_ERR 4
#define AL_MAX 5

#define AL_LOG_ENABLED	1
#define AL_LOG_SRC	1
#define AL_LOG_LEVEL	AL_DBG

#if AL_LOG_ENABLED
#if AL_LOG_SRC
#define AL_LOG(level, fmt, ...)  {	\
	if (level >= AL_LOG_LEVEL)	\
		al_log(level, __FILE__, __LINE__, __FUNCTION__,\
				fmt, ##__VA_ARGS__);	\
}
#else
#define AL_LOG(level, fmt, ...)  {	\
	if (level >= AL_LOG_LEVEL)	\
		al_log(level, "unk", 0, "unk", fmt, ##__VA_ARGS__);	\
}
#endif
#else
#define AL_LOG(level, fmt, ...)
#endif

