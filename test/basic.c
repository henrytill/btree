#include <assert.h>
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

	err = btree_txn_get(tree, txn, &key, &actual);
	if (err == BT_FAIL) {
		ret = FAILURE;
		goto out;
	}

	err = btree_txn_commit(txn);
	if (err == BT_FAIL) {
		ret = FAILURE;
		goto out;
	}

	if (expected.size != actual.size) {
		ret = FAILURE;
		goto out;
	}

	if (strcmp(expected.data, actual.data) != 0) {
		ret = FAILURE;
		goto out;
	}

	btval_reset(&actual);

	if (expected.size == actual.size) {
		ret = FAILURE;
		goto out;
	}

	txn = btree_txn_begin(tree, true);
	if (txn == NULL) {
		ret = FAILURE;
		goto out;
	}

	err = btree_txn_get(tree, txn, &key, &actual);
	if (err == BT_FAIL) {
		ret = FAILURE;
		goto out;
	}

	if (expected.size != actual.size) {
		ret = FAILURE;
		goto out;
	}

	if (strcmp(expected.data, actual.data) != 0) {
		ret = FAILURE;
		goto out;
	}

	ret = SUCCESS;
out:
	btree_txn_abort(txn);
	btree_close(tree);
	return ret;
}
