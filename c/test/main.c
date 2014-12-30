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

static int fd_read(int fd, void *buf, size_t len)
{
        int err;
        ssize_t ret;
        size_t pos = 0;
        while (pos < len) {
                ret = read(fd, (char *)buf + pos, len - pos);
                if (ret < 0) {
                        err = errno;
                        goto out;
                }
                if (ret == 0) {
                        err = -EIO;
                        goto out;
                }
                pos += ret;
        }
        err = 0;
out:
        return err;
}

static int lrandom_buf(void *ptr, size_t size)
{
        int fd = open("/dev/urandom", O_RDONLY);
        int err;

        if (fd < 0) {
		err = errno;
                return err;
	}

        err = fd_read(fd, ptr, size);
        close(fd);
        return err;
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
	rt.random_buf = lrandom_buf;
	al_init(&rt);
}

static int test_btree_insert()
{
	struct btree *t;
	struct btree_key *key;
	struct btree_value *value;
	int i;
	char *key_hex;

	t = btree_create();
	if (!t) {
		printf("cant create tree\n");
		return -1;
	}
	printf("t=%p, root=%p sz=%zu\n", t, t->root, sizeof(*t->root));

	for (i = 0; i < 100000; i++) {
		key = btree_gen_key();
		value = btree_gen_value();
		key_hex = btree_key_hex(key);
		if (!key || !value) {
			printf("cant gen key value\n");
			return -1;
		}

		key_hex = btree_key_hex(key);
		if (!key_hex) {
			printf("cant get key hex\n");
			return -1;
		}

		printf("inserting key[%d] %s\n", i, key_hex);
		if (btree_insert_key(t, key, value)) {
			char *key_hex = btree_key_hex(key);
			if (!key_hex) {
				printf("cant get key hex\n");
				return -1;
			}
			printf("cant insert key %s\n", key_hex);
			if (key_hex)
				al_free(key_hex);
		}
		al_free(key);
		al_free(value);
		al_free(key_hex);
	}

	btree_log(t);
	printf("tree nr_keys=%lu\n", btree_nr_keys(t));
	btree_delete(t);
	return 0;
}

static int test_btree()
{
	int rc;
	rc = test_btree_insert();
	if (!rc)
		return rc;

	return 0;
}

int main(int argc, char *argv[])
{
	int rc;

	prepare();

	rc = test_btree();
	if (!rc)
		exit(rc);

	return 0;
}
