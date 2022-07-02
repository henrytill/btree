#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "test.h"

#define FLAGS 0
#define MODE  0

int
main(int argc, char *argv[])
{
	char path[] = TEMPLATE;
	const char *actual_path = NULL;
	struct btree *tree = NULL;

	if (mkstemp(path) == -1) {
		exit(1);
	}

	DPRINTF("db path: %s", path);

	if ((tree = btree_open(path, FLAGS, MODE)) == NULL) {
		goto fail;
	}

	actual_path = btree_get_path(tree);

	assert(strcmp(path, actual_path) == 0);

	btree_close(tree);
	exit(0);

fail:
	btree_close(tree);
	exit(1);
}
