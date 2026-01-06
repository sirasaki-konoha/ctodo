#ifndef _INTERNAL_WRITE_H
#define _INTERNAL_WRITE_H
#include <stdio.h>

typedef struct {
    FILE* fp;
    char* context;
} Writer;

Writer wopen(const char* path);

int add_string(Writer* wt, const char* str);

void flush_write(Writer* wt);

void free_writer(Writer* wt);

#endif /* _INTERNAL_WRITE_H */
