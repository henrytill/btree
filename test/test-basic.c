#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "test.h"

int
main(int argc, char *argv[])
{
	char path[] = TEMPLATE;
	int fd;
	struct btree *tree = NULL;
	struct btree_txn *txn = NULL;
	struct btval key;
	struct btval expected;
	struct btval actual;

	key.size = strlen(KEY_HELLO);
	key.data = KEY_HELLO;

	expected.size = strlen(VAL_WORLD);
	expected.data = VAL_WORLD;

	if ((fd = mkstemp(path)) == -1) {
		exit(1);
	}

	DPRINTF("db path: %s", path);

	if ((tree = btree_open_fd(fd, 0)) == NULL) {
		goto fail;
	}

	if ((txn = btree_txn_begin(tree, false)) == NULL) {
		goto fail;
	}

	if (btree_txn_put(tree, txn, &key, &expected, 0) == BT_FAIL) {
		goto fail;
	}

	if (btree_txn_get(tree, txn, &key, &actual) == BT_FAIL) {
		goto fail;
	}

	if (btree_txn_commit(txn) == BT_FAIL) {
		goto fail;
	}

	assert(expected.size == actual.size);
	assert(strcmp(expected.data, actual.data) == 0);

	btval_reset(&actual);

	assert(expected.size != actual.size);

	if ((txn = btree_txn_begin(tree, true)) == NULL) {
		goto fail;
	}

	if (btree_txn_get(tree, txn, &key, &actual) == BT_FAIL) {
		goto fail;
	}

	assert(expected.size == actual.size);
	assert(strcmp(expected.data, actual.data) == 0);

	btree_txn_abort(txn);
	btree_close(tree);
	return 0;

fail:
	btree_txn_abort(txn);
	btree_close(tree);
	exit(1);
}
