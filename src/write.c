#include "write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kutil.h"

Writer wopen(const char* path)
{
    FILE* fp = fopen(path, "r+");

    Writer wt = { .context = format_string(""), .fp = fp };
    return wt;
}

int add_string(Writer* wt, const char* str)
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

void flush_write(Writer* wt)
{
    fputs(wt->context, wt->fp);
    return;
}

void free_writer(Writer* wt)
{
    free(wt->context);
    fclose(wt->fp);
}
