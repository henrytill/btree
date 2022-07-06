#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "test.h"

#define TEMPLATE "/tmp/treeXXXXXX"

enum {
	FLAGS = 0,
	MODE = 0,
};

int
main(int argc, char *argv[])
{
	char path[] = TEMPLATE;
	const char *actual_path = NULL;
	struct btree *tree = NULL;
	int ret = FAILURE;

	if (mkstemp(path) == -1) {
		return FAILURE;
	}

	fprintf(stderr, "db path: %s", path);

	if ((tree = btree_open(path, FLAGS, MODE)) == NULL) {
		ret = FAILURE;
		goto out;
	}

	actual_path = btree_get_path(tree);
	if (strcmp(path, actual_path) != 0) {
		ret = FAILURE;
		goto out;
	}

	ret = SUCCESS;
out:
	btree_close(tree);
	return ret;
}
