#include "json_read.h"

#include <stdlib.h>

#include "kutil.h"
#include "parson.h"

TodoJson parse_todo(const char* todo_content)
{
    TodoJson td;
    JSON_Value* root;
    JSON_Object* obj;
    JSON_Array* arr;
    size_t count;

    root = json_parse_string(todo_content);
    obj = json_value_get_object(root);
    arr = json_object_get_array(obj, "todos");

    count = json_array_get_count(arr);

    for (size_t i = 0; i < count; i++) {
        JSON_Object* arrobj = json_array_get_object(arr, i);

        const char* title = json_object_get_string(arrobj, "title");
        const char* created_at = json_object_get_string(arrobj, "created_at");

        td.len += 1;
        td.todo[i].title = title ? safe_strdup(title) : NULL;
        td.todo[i].created_at = created_at ? safe_strdup(created_at) : NULL;
        td.todo[i].id = json_object_get_number(arrobj, "id");
        td.todo[i].done = json_object_get_boolean(arrobj, "done");
    }

    json_value_free(root);
    return td;
}

void free_todo(TodoJson* td)
{
    for (int i = 0; i < td->len; i++) {
        free(td->todo[i].title);
        free(td->todo[i].created_at);
    }
}
