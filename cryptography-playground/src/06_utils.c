#include "../header.h"

void clear_input(void) { while (getchar() != '\n'); }

void print_hex(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("%02x", data[i]);
    printf("\n");
}

void *xmalloc(size_t sz)
{
    void *p = malloc(sz);
    if (!p) { fprintf(stderr, "malloc failed\n"); exit(1); }
    return p;
}
