#include <stdlib.h>
#include <time.h>

#include "json_read.h"
#include "kutil.h"
#include "read.h"
#include "utils.h"

char* get_time()
{
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);

    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    int day = tm->tm_mday;

    char* time = format_string("%d-%d-%d",
        year,
        month,
        day);

    return time;
}

TodoJson load_todos()
{
    char* content = read_file("test/todo.json");
    TodoJson td = parse_todo((const char*)content);

    free(content);
    return td;
}
