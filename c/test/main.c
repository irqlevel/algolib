#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <algolib/include/algolib.h>

static u32 lgetpid(void)
{
	return (u32)getpid();
}

static void ltime(struct al_time *time)
{
	struct timeval tv;
	struct tm tm;
	time_t secs;

	gettimeofday(&tv, NULL);
	secs = tv.tv_sec;
	gmtime_r(&secs, &tm);

	time->year = tm.tm_year;
	time->mon = tm.tm_mon;
	time->day = tm.tm_mday;
	time->hour = tm.tm_hour;
	time->min = tm.tm_min;
	time->sec = tm.tm_sec;
	time->usec = tv.tv_usec;
}

static void prepare(void)
{
	struct al_crt rt;

	rt.malloc = malloc;
	rt.free = free;
	rt.memcmp = memcmp;
	rt.memcpy = memcpy;
	rt.memset = memset;
	rt.lprintf = printf;
	rt.vsnprintf = vsnprintf;
	rt.strrchr = strrchr;
	rt.getpid = lgetpid;
	rt.time = ltime;

	al_init(&rt);
}

int main(int argc, char *argv[])
{
	struct btree *t;

	prepare();	

	t = btree_create();
	if (!t) {
		printf("cant create tree\n");
		exit(-1);
	}
	printf("t=%p, root=%p sz=%zu\n", t, t->root, sizeof(*t->root));
	btree_delete(t);
	return 0;
}
