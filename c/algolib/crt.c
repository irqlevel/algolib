#include <include/crt.h>

static char *al_log_level_s[] = {"INV", "DBG", "INF" , "WRN" , "ERR", "MAX"};

static struct al_crt g_rt;

void al_init(struct al_crt *rt)
{
	g_rt.malloc = rt->malloc;
	g_rt.free = rt->free;
	g_rt.memset = rt->memset;
	g_rt.memcpy = rt->memcpy;
	g_rt.memcmp = rt->memcmp;
	g_rt.lprintf = rt->lprintf;
	g_rt.time = rt->time;
	g_rt.vsnprintf = rt->vsnprintf;
	g_rt.strrchr = rt->strrchr;
	g_rt.getpid = rt->getpid;
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


static void al_log_write_fmt_args(char **buff, int *left, const char *fmt, va_list args)
{
	int res;

	if (*left < 0)
		return ;

	res = get_rt()->vsnprintf(*buff,*left,fmt,args);
	if (res > 0) {
		*buff+=res;
		*left-=res;
	}
	return;
}

static void al_log_write_fmt(char **buff, int *left, const char *fmt, ...)
{
	va_list args;

	va_start(args,fmt);
	al_log_write_fmt_args(buff, left,fmt,args);
	va_end(args);
}

static const char * al_truncate_file_path(const char *filename)
{
	char *pos;
	pos = get_rt()->strrchr(filename, '/');
	if (pos)
		return ++pos;
	else
		return filename;
}

void al_log(int level, const char *file, int line, const char *func,
	const char *fmt, ...)
{
	char buf[PAGE_SIZE];
	char *pos, *begin;
	int left, count;
	char *level_s;
	struct al_time tm;
	va_list args;

	if (level < 0 || level >= ARRAY_SIZE(al_log_level_s))
		return;

	level_s = al_log_level_s[level];
	begin = buf;
	pos = begin;
	count = PAGE_SIZE/sizeof(char);
	left = count - 1;
	get_rt()->time(&tm);

	al_log_write_fmt(&pos,&left,
		"%04d-%02d-%02d %02d:%02d:%02d.%.6ld %s p%u %s %u %s() ",
		1900+tm.year, tm.mon+1,
		tm.day, tm.hour, tm.min, tm.sec,
		tm.usec, level_s, get_rt()->getpid(),
		al_truncate_file_path(file), line, func);

	va_start(args,fmt);
	al_log_write_fmt_args(&pos,&left,fmt,args);
	va_end(args);

	begin[count-1] = '\0';
	get_rt()->lprintf("%s\n", begin);
}
