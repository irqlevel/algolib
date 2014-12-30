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

static int test_btree_insert(int num_keys)
{
	struct btree *t = NULL;
	struct btree_key **keys = NULL;
	struct btree_value **values = NULL;
	struct btree_value *value = NULL;
	int i;
	int err;
	char *key_hex = NULL, *val_hex = NULL;

	keys = malloc(num_keys*sizeof(struct btree_key *));
	if (!keys) {
		printf("cant malloc keys\n");
		err = -ENOMEM;
		goto cleanup;
	}
	memset(keys, 0, num_keys*sizeof(struct btree_key *));

	values = malloc(num_keys*sizeof(struct btree_value *));
	if (!values) {
		printf("cant malloc values\n");
		err = -ENOMEM;
		goto cleanup;
	}
	memset(values, 0, num_keys*sizeof(struct btree_value *));

	for (i = 0; i < num_keys; i++) {
		keys[i] = btree_gen_key();
		if (!keys[i]) {
			printf("cant gen key[%d]\n", i);
			err = -ENOMEM;
			goto cleanup;
		}
		values[i] = btree_gen_value();
		if (!values[i]) {
			printf("cant gen value[%d]\n", i);
			err = -ENOMEM;
			goto cleanup;
		}
	}

	t = btree_create();
	if (!t) {
		printf("cant create tree\n");
		err = -ENOMEM;
		goto cleanup;
	}
	printf("t=%p, root=%p sz=%zu\n",
			t, t->root, sizeof(*t->root));

	/* insert keys */
	for (i = 0; i < num_keys; i++) {
		key_hex = btree_key_hex(keys[i]);
		if (!key_hex) {
			printf("cant get key hex\n");
			err = -ENOMEM;
			goto cleanup;
		}
		val_hex = btree_value_hex(values[i]);
		if (!val_hex) {
			printf("cant get val hex\n");
			err = -ENOMEM;
			goto cleanup;
		}

		err = btree_insert_key(t, keys[i], values[i]);
		if (err) {
			printf("Cant insert key[%d] %s, rc=%d\n",
					i, key_hex, err);
			goto cleanup;
		}

		printf("Insert key[%d] %s -> %s, rc=%d\n",
				i, key_hex, val_hex, err);

		al_free(key_hex);
		key_hex = NULL;
		al_free(val_hex);
		val_hex = NULL;
	}

	/* now check keys exists in btree */
	for (i = 0; i < num_keys; i++) {
		key_hex = btree_key_hex(keys[i]);
		if (!key_hex) {
			printf("cant get key hex\n");
			err = -ENOMEM;
			goto cleanup;
		}

		err = btree_find_key(t, keys[i], &value);
		if (err) {
			printf("Cant found key[%d] %s, rc=%d\n",
					i, key_hex, err);
			goto cleanup;
		}

		val_hex = btree_value_hex(value);
		if (!val_hex) {
			printf("cant get val hex\n");
			err = -ENOMEM;
			goto cleanup;
		}

		printf("Found key[%d] %s -> %s, rc=%d\n",
				i, key_hex, val_hex, err);
		if (0 != memcmp(values[i], value, sizeof(*value))) {
			printf("value mismatch\n");
			err = -EINVAL;
			goto cleanup;
		}
		al_free(value);
		value = NULL;
		al_free(key_hex);
		key_hex = NULL;
		al_free(val_hex);
		val_hex = NULL;
	}

	/* output some stats */
	btree_log(t);
	printf("tree nr_keys=%lu\n", btree_nr_keys(t));

	err = 0;
cleanup:
	if (value)
		al_free(value);
	if (key_hex)
		al_free(key_hex);
	if (val_hex)
		al_free(val_hex);

	if (values) {
		for (i = 0; i < num_keys; i++)
			if (values[i])
				al_free(values[i]);
		free(values);
	}

	if (keys) {
		for (i = 0; i < num_keys; i++)
			if (keys[i])
				al_free(keys[i]);
		free(keys);
	}

	if (t)
		btree_delete(t);
	return 0;
}

static int test_btree()
{
	int rc;
	rc = test_btree_insert(100000);
	if (rc)
		return rc;

	return 0;
}

int main(int argc, char *argv[])
{
	int rc;

	prepare();

	rc = test_btree();
	if (rc)
		exit(rc);

	return 0;
}
