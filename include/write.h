#ifndef _INTERNAL_WRITE_H
#define _INTERNAL_WRITE_H
#include <stdio.h>

typedef struct {
    FILE* fp;
    char* context;
} Writer;

Writer w_open(const char* path);
Writer w_create(const char* path);

int w_add_string(Writer* wt, const char* str);

void w_flush_write(Writer* wt);

void w_free_writer(Writer* wt);

#endif /* _INTERNAL_WRITE_H */
