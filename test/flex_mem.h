#ifndef BTREE_MISC_FLEXIBLE_ARRAY_MEMBER_H
#define BTREE_MISC_FLEXIBLE_ARRAY_MEMBER_H

/* clang-format off */

/* xxd -i <(echo -n 'hello, world!\0') */
const unsigned char hello[] = {
        0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64,
        0x21, 0x00
};

const unsigned int hello_len = 14;

/* xxd -i <(echo -n 'goodbye, world!\0') */
const unsigned char goodbye[] = {
        0x67, 0x6f, 0x6f, 0x64, 0x62, 0x79, 0x65, 0x2c, 0x20, 0x77, 0x6f, 0x72,
        0x6c, 0x64, 0x21, 0x00
};

const unsigned int goodbye_len = 16;

/* clang-format on */

enum {
	SUCCESS = 0,
	FAILURE = 1,
};

struct flex_mem {
	unsigned int size;
	unsigned char data[];
};

int
flex_mem_new(const unsigned char *array,
             unsigned int array_size,
             struct flex_mem **p);

int
flex_mem_set(const unsigned char *array,
             unsigned int array_size,
             struct flex_mem **p);

struct flex_mem_alt {
	unsigned int size;
	unsigned char data[1];
};

struct flex_mem_old {
	unsigned int size;
	unsigned char data[0];
};

#endif
