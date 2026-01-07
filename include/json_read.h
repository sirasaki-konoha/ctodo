#ifndef _INTERNAL_JSON_READ
#define _INTERNAL_JSON_READ

typedef struct Todos {
    int id;
    char* title;
    char* created_at;
    int done;
} Todos;

typedef struct {
    int len;
    Todos todo[500];
} TodoJson;

TodoJson parse_todo(const char* todo_content);

void free_todo(TodoJson* td);

#endif /* _INTERNAL_JSON_READ */
