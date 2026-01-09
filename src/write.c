#include "write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kutil.h"

Writer w_open(const char* path)
{
    FILE* fp = fopen(path, "r+");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    Writer wt = { .context = format_string(""), .fp = fp };
    return wt;
}

Writer w_create(const char* path)
{
    FILE* fp = fopen(path, "w+");

    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    Writer wt = { .context = format_string(""), .fp = fp };
    return wt;
}

int w_add_string(Writer* wt, const char* str)
{
    char* old = safe_strdup(wt->context);
    if (old == NULL) {
        fprintf(stderr, "Allocate failed: safe_strdup");
        return -1;
    }

    free(wt->context);

    wt->context = format_string("%s%s", old, str);
    free(old);

    return 0;
}

void w_flush_write(Writer* wt)
{
    fputs(wt->context, wt->fp);
    return;
}

void w_free_writer(Writer* wt)
{
    free(wt->context);
    fclose(wt->fp);
}
