#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "test.h"

#define TEMPLATE "/tmp/treeXXXXXX"

int
main(int argc, char *argv[])
{
	char *key_hello = "hello";
	char *val_world = "world";
	struct btree *tree = NULL;
	struct btree_txn *txn = NULL;
	struct btval key = {
	    .size = strlen(key_hello),
	    .data = key_hello,
	};
	struct btval expected = {
	    .size = strlen(val_world),
	    .data = val_world,
	};
	struct btval actual = {
	    .size = 0,
	    .data = NULL,
	};
	char path[] = TEMPLATE;
	int ret = FAILURE;
	int err = BT_FAIL;

	const int fd = mkstemp(path);
	if (fd == -1) {
		return FAILURE;
	}

	fprintf(stderr, "db path: %s", path);

	tree = btree_open_fd(fd, 0);
	if (tree == NULL) {
		ret = FAILURE;
		goto out;
	}

	txn = btree_txn_begin(tree, false);
	if (txn == NULL) {
		ret = FAILURE;
		goto out;
	}

	err = btree_txn_put(tree, txn, &key, &expected, 0);
	if (err == BT_FAIL) {
		ret = FAILURE;
		goto out;
	}

	/*
	 * Adding the following stanza will trip an assertion in btree_txn_abort
	 * which stipulates that the reference count for a given page in the
	 * dirty queue is 0, because a successful btree_txn_get will bump the
	 * reference count for a given page.
	 */

	/* err = btree_txn_get(tree, txn, &key, &actual);
	 * if (err == BT_FAIL) {
	 * 	ret == FAILURE;
	 * 	goto out;
	 * }
	 */

	btree_txn_abort(txn);

	txn = btree_txn_begin(tree, false);
	if (txn == NULL) {
		ret = FAILURE;
		goto out;
	}

	err = btree_txn_get(tree, txn, &key, &actual);
	if (err != BT_FAIL || errno != ENOENT) {
		ret = FAILURE;
		goto out;
	}

	ret = SUCCESS;
out:
	btree_txn_abort(txn);
	btree_close(tree);
	return ret;
}
