#ifndef _INTERNAL_JSON_WRITE
#define _INTERNAL_JSON_WRITE
#include "json_read.h"

char* create_json(TodoJson td);

int todo_push(TodoJson* td, Todos t);

Todos new_todo(const char* title, const char* created_at, int done);

void free_json(char* json);

#endif /* _INTERNAL_JSON_WRITE */
