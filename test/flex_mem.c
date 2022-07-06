#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flex_mem.h"
#include "test.h"

/* Implementation */

int
flex_mem_new(const unsigned char *array,
             unsigned int array_size,
             struct flex_mem **p)
{
	*p = malloc(sizeof(struct flex_mem) +
	            sizeof(unsigned char) * array_size);
	if (*p == NULL) {
		return FAILURE;
	}

	(*p)->size = array_size;

	memcpy((*p)->data, array, array_size);

	return SUCCESS;
}

int
flex_mem_set(const unsigned char *array,
             unsigned int array_size,
             struct flex_mem **p)
{
	struct flex_mem *p_resized = NULL;

	if (array_size == (*p)->size) {
		return SUCCESS;
	}

	p_resized = realloc(*p, array_size);
	if (p_resized == NULL) {
		return FAILURE;
	}

	*p = p_resized;
	(*p)->size = array_size;
	memcpy((*p)->data, array, array_size);

	return SUCCESS;
}

/* Test Code */

static void
flex_mem_info(void)
{
	size_t flex_mem_size = sizeof(struct flex_mem);
	size_t flex_mem_alt_size = sizeof(struct flex_mem_alt);
	size_t flex_mem_old_size = sizeof(struct flex_mem_old);

	printf("flex_mem: %li\n", flex_mem_size);
	printf("flex_mem_alt: %li\n", flex_mem_alt_size);
	printf("flex_mem_old: %li\n", flex_mem_old_size);
}

static int
flex_mem_test(void)
{
	struct flex_mem *p = NULL;
	const char *const expected_hello = "hello, world!";
	const char *const expected_goodbye = "goodbye, world!";
	int ret = FAILURE;

	if (flex_mem_new(hello, hello_len, &p) != 0) {
		ret = FAILURE;
		goto out;
	}

	if (memcmp(expected_hello, p->data, p->size) != 0) {
		ret = FAILURE;
		goto out;
	}

	if (flex_mem_set(goodbye, goodbye_len, &p) != 0) {
		ret = FAILURE;
		goto out;
	}

	if (memcmp(expected_goodbye, p->data, p->size) != 0) {
		ret = FAILURE;
		goto out;
	}

	ret = SUCCESS;
out:
	if (p != NULL) {
		free(p);
	}
	return ret;
}

int
main(int argc, char *argv[])
{
	flex_mem_info();

	return flex_mem_test();
}
