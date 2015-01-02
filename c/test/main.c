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
	AL_LOG(AL_TST, "Prepared");
}


static int test_btree_insert_case(struct btree *t, struct btree_key **keys,
		struct btree_value **values, int num_keys, int replace,
		int fail_ok)
{
	int i;
	int err;
	char *key_hex = NULL, *val_hex = NULL;

	/* insert keys */
	for (i = 0; i < num_keys; i++) {
		key_hex = btree_key_hex(keys[i]);
		if (!key_hex) {
			AL_LOG(AL_ERR, "cant get key hex");
			err = -ENOMEM;
			goto cleanup;
		}
		val_hex = btree_value_hex(values[i]);
		if (!val_hex) {
			AL_LOG(AL_ERR, "cant get val hex");
			err = -ENOMEM;
			goto cleanup;
		}

		err = btree_insert_key(t, keys[i], values[i], replace);
		if (err && !fail_ok) {
			AL_LOG(AL_ERR, "Cant insert key[%d] %s rplc=%d rc=%d",
					i, key_hex, replace, err);
			goto cleanup;
		}

		AL_LOG(AL_TST, "Insert key[%d] %s -> %s rplc=%d rc=%d",
				i, key_hex, val_hex, replace, err);

		al_free(key_hex);
		key_hex = NULL;
		al_free(val_hex);
		val_hex = NULL;
	}

	err = 0;
cleanup:
	if (key_hex)
		al_free(key_hex);
	if (val_hex)
		al_free(val_hex);
	return err;	
}

static int test_btree_find_case(struct btree *t, struct btree_key **keys,
		struct btree_value **values, int num_keys)
{
	int i;
	int err;
	char *key_hex = NULL, *val_hex = NULL;
	struct btree_value *value = NULL;

	for (i = 0; i < num_keys; i++) {
		key_hex = btree_key_hex(keys[i]);
		if (!key_hex) {
			AL_LOG(AL_ERR, "cant get key hex");
			err = -ENOMEM;
			goto cleanup;
		}

		err = btree_find_key(t, keys[i], &value);
		if (err) {
			AL_LOG(AL_ERR, "Cant found key[%d] %s, rc=%d",
					i, key_hex, err);
			goto cleanup;
		}

		val_hex = btree_value_hex(value);
		if (!val_hex) {
			AL_LOG(AL_ERR, "cant get val hex");
			err = -ENOMEM;
			goto cleanup;
		}

		AL_LOG(AL_TST, "Found key[%d] %s -> %s, rc=%d",
				i, key_hex, val_hex, err);
		if (0 != memcmp(values[i], value, sizeof(*value))) {
			AL_LOG(AL_ERR, "value mismatch");
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

	err = 0;
cleanup:
	if (value)
		al_free(value);
	if (key_hex)
		al_free(key_hex);
	if (val_hex)
		al_free(val_hex);
	return err;	
}

static int test_btree_delete_case(struct btree *t, struct btree_key **keys,
		struct btree_value **values, int num_keys)
{
	int i;
	int err;
	char *key_hex = NULL;

	for (i = 0; i < num_keys; i++) {
		key_hex = btree_key_hex(keys[i]);
		if (!key_hex) {
			AL_LOG(AL_ERR, "cant get key hex");
			err = -ENOMEM;
			goto cleanup;
		}

		err = btree_delete_key(t, keys[i]);
		if (err) {
			AL_LOG(AL_ERR, "Cant delete key[%d] %s, rc=%d",
					i, key_hex, err);
		} else {
			AL_LOG(AL_TST, "Deleted key[%d] %s, rc=%d", i, key_hex, err);
		}
		al_free(key_hex);
		key_hex = NULL;
	}

	err = 0;
cleanup:
	if (key_hex)
		al_free(key_hex);
	return err;	
}


static int test_btree_stats_case(struct btree *t)
{
	struct btree_info info;
	/* output some stats */
	btree_log(t, AL_TST);
	btree_stats(t, &info);

	AL_LOG(AL_TST, "tree %p nr_keys=%llu nr_nodes=%llu",
		t, info.nr_keys, info.nr_nodes);

	return 0;
}

static int __test_btree(int num_keys)
{
	struct btree *t = NULL;
	struct btree_key **keys = NULL;
	struct btree_value **values = NULL;
	int i;
	int err;

	keys = malloc(num_keys*sizeof(struct btree_key *));
	if (!keys) {
		AL_LOG(AL_ERR, "cant malloc keys");
		err = -ENOMEM;
		goto cleanup;
	}
	memset(keys, 0, num_keys*sizeof(struct btree_key *));

	values = malloc(num_keys*sizeof(struct btree_value *));
	if (!values) {
		AL_LOG(AL_ERR,"cant malloc values");
		err = -ENOMEM;
		goto cleanup;
	}
	memset(values, 0, num_keys*sizeof(struct btree_value *));

	for (i = 0; i < num_keys; i++) {
		keys[i] = btree_gen_key();
		if (!keys[i]) {
			AL_LOG(AL_ERR, "cant gen key[%d]", i);
			err = -ENOMEM;
			goto cleanup;
		}
		values[i] = btree_gen_value();
		if (!values[i]) {
			AL_LOG(AL_ERR, "cant gen value[%d]", i);
			err = -ENOMEM;
			goto cleanup;
		}
	}

	t = btree_create();
	if (!t) {
		AL_LOG(AL_ERR, "cant create tree");
		err = -ENOMEM;
		goto cleanup;
	}
	AL_LOG(AL_TST, "t=%p, root=%p sz=%zu",
			t, t->root, sizeof(*t->root));

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* insert keys */
	err = test_btree_insert_case(t, keys, values, num_keys, 0, 0);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}


	/* output tree structures and stats */
	err = test_btree_stats_case(t);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* insert keys again */
	err = test_btree_insert_case(t, keys, values, num_keys, 0, 1);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}


	/* output tree structures and stats */
	err = test_btree_stats_case(t);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* replace keys */
	err = test_btree_insert_case(t, keys, values, num_keys, 1, 0);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* output tree structures and stats */
	err = test_btree_stats_case(t);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* find keys */
	err = test_btree_find_case(t, keys, values, num_keys);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* output tree structures and stats */
	err = test_btree_stats_case(t);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* delete keys */
	err = test_btree_delete_case(t, keys, values, num_keys);
	if (err)
		goto cleanup;
	
	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	/* output tree structures and stats */
	err = test_btree_stats_case(t);
	if (err)
		goto cleanup;

	err = btree_check(t);
	if (err) {
		AL_LOG(AL_ERR, "t=%p check errs=%d", t, err);
		goto cleanup;
	}

	AL_LOG(AL_TST, "PASSED");

	err = 0;
cleanup:
	if (err)
		AL_LOG(AL_TST, "FAILED");

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
	rc = __test_btree(10000);
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
