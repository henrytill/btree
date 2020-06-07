#ifndef BTREE_TEST_H
#define BTREE_TEST_H

#ifndef NDEBUG
#include <stdio.h>
#endif

#ifndef NDEBUG
#define DPRINTF(...)                                                           \
	do {                                                                   \
		fprintf(stderr, "%s:%d: ", __func__, __LINE__);                \
		fprintf(stderr, __VA_ARGS__);                                  \
		fprintf(stderr, "\n");                                         \
	} while (0)
#else
#define DPRINTF(...)
#endif

#define TEMPLATE  "/tmp/treeXXXXXX"
#define KEY_HELLO "hello"
#define VAL_WORLD "world"

#endif
