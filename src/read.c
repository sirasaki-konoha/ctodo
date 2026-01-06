#include "read.h"

#include <stdio.h>
#include <stdlib.h>

// Plese free after use
char* read_file(const char* file_path)
{
    FILE* fp;
    char* buf;

    fp = fopen(file_path, "r");
    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    buf = malloc(size + 1);
    if (!buf) {
        perror("malloc");
        return NULL;
    }

    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);

    return buf;
}
