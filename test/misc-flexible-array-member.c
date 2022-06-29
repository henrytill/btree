#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "misc-flexible-array-member.h"

/* Implementation */

int
flex_mem_new(const unsigned char *array,
             unsigned int         array_size,
             struct flex_mem **   p)
{
	*p = (struct flex_mem *)malloc(sizeof(struct flex_mem) +
	                               sizeof(unsigned char) * array_size);
	if (*p == NULL) {
		return -1;
	}

	(*p)->size = array_size;

	memcpy((*p)->data, array, array_size);

	return 0;
}

int
flex_mem_set(const unsigned char *array,
             unsigned int         array_size,
             struct flex_mem **   p)
{
	struct flex_mem *structp_resized = NULL;

	if (array_size != (*p)->size) {
		structp_resized = (struct flex_mem *)realloc(*p, array_size);
		if (structp_resized == NULL) {
			return -1;
		}
		*p = structp_resized;
		(*p)->size = array_size;
	}

	memcpy((*p)->data, array, array_size);

	return 0;
}

/* Test Code */

static void
flex_mem_info(void)
{
	size_t unsigned_init_size = sizeof(unsigned int);
	size_t unsigned_char_size = sizeof(unsigned char);

	printf("unsigned int: %li\n", unsigned_init_size);
	printf("unsigned char: %li\n", unsigned_char_size);

	size_t flex_mem_size = sizeof(struct flex_mem);
	size_t flex_mem_alt_size = sizeof(struct flex_mem_alt);
	size_t flex_mem_old_size = sizeof(struct flex_mem_old);

	printf("flex_mem: %li\n", flex_mem_size);
	printf("flex_mem_alt: %li\n", flex_mem_alt_size);
	printf("flex_mem_old: %li\n", flex_mem_old_size);
}

static bool
flex_mem_test(void)
{
	struct flex_mem *   structp = NULL;
	const unsigned char expected_hello[] = "hello, world!";
	const unsigned char expected_goodbye[] = "goodbye, world!";
	bool                ret = false;

	if (flex_mem_new(hello, hello_len, &structp) != 0) {
		goto cleanup;
	}

	if (memcmp(expected_hello, structp->data, structp->size) != 0) {
		goto cleanup;
	}

	if (flex_mem_set(goodbye, goodbye_len, &structp) != 0) {
		goto cleanup;
	}

	if (memcmp(expected_goodbye, structp->data, structp->size) != 0) {
		goto cleanup;
	}

	ret = true;

cleanup:
	if (structp != NULL) {
		free(structp);
	}
	return ret;
}

int
main(int argc, char *argv[])
{
	flex_mem_info();

	assert(flex_mem_test());

	return EXIT_SUCCESS;
}
