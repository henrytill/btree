#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

#include "btree.h"

#ifndef NDEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define DPRINTF(...)                                                           \
	do {                                                                   \
		fprintf(stderr, "%s:%d: ", __func__, __LINE__);                \
		fprintf(stderr, __VA_ARGS__);                                  \
		fprintf(stderr, "\n");                                         \
	} while (0)
#else
#define DPRINTF(...)
#endif

#define TEMPLATE "/tmp/treeXXXXXX"
#define KEY      "hello"
#define VALUE    "world"

int
main(int argc, char *argv[])
{
	char              path[] = TEMPLATE;
	int               fd;
	struct btree *    tree;
	struct btree_txn *txn;
	struct btval      key;
	struct btval      expected;
	struct btval      actual;

	key.size = strlen(KEY);
	key.data = KEY;

	expected.size = strlen(VALUE);
	expected.data = VALUE;

	if ((fd = mkstemp(path)) == -1) {
		exit(1);
	}

	DPRINTF("db path: %s", path);

	if ((tree = btree_open_fd(fd, 0)) == NULL) {
		exit(1);
	}

	if ((txn = btree_txn_begin(tree, false)) == NULL) {
		goto fail;
	}

	if (btree_txn_put(tree, txn, &key, &expected, 0) == BT_FAIL) {
		goto fail;
	}

	/*
	 * Adding the following stanza will trip an assertion in btree_txn_abort
	 * which stipulates that reference count for a given page in the dirty
	 * queue is 0, because a successful btree_txn_get will bump the
	 * reference count for a given page.
	 */

	/*
	 * if (btree_txn_get(tree, txn, &key, &actual) == BT_FAIL) {
	 * 	goto fail;
	 * }
	 *
	 * assert(expected.size == actual.size);
	 * assert(strcmp(expected.data, actual.data) == 0);
	 */

	btree_txn_abort(txn);

	if ((txn = btree_txn_begin(tree, false)) == NULL) {
		goto fail;
	}

	assert(btree_txn_get(tree, txn, &key, &actual) == BT_FAIL);
	assert(errno == ENOENT);

	btree_txn_abort(txn);
	btree_close(tree);
	return 0;

fail:
	btree_txn_abort(txn);
	btree_close(tree);
	exit(1);
}
