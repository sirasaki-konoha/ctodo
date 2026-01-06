#include "json_write.h"

#include <stdio.h>
#include <stdlib.h>

#include "json_read.h"
#include "kutil.h"
#include "parson.h"

char* create_json(TodoJson td)
{
    JSON_Value* root;
    JSON_Object* obj;
    JSON_Value* arr_val;
    JSON_Array* arr;
    char* json;

    root = json_value_init_object();
    obj = json_value_get_object(root);
    arr_val = json_value_init_array();

    json_object_set_value(obj, "todos", arr_val);

    arr = json_object_get_array(obj, "todos");

    for (int i = 0; i < td.len; i++) {
        JSON_Value* val_obj = json_value_init_object();
        json_array_append_value(arr, val_obj);

        JSON_Object* obj_arr = json_array_get_object(arr, i);

        json_object_set_string(obj_arr, "title", td.todo[i].title);
        json_object_set_string(obj_arr, "created_at", td.todo[i].created_at);
        json_object_set_boolean(obj_arr, "done", td.todo[i].done);
    }

    json = json_serialize_to_string_pretty(root);
    json_value_free(root);

    return json;
}

Todos new_todo(const char* title, const char* created_at, int done)
{
    Todos td = {
        .title = safe_strdup(title),
        .created_at = safe_strdup(created_at),
        .done = done,
    };

    return td;
}

int todo_push(TodoJson* td, Todos t)
{
    if (td->len >= 500)
        return -1;
    td->todo[td->len++] = t;
    return 0;
}

void free_json(char* json)
{
    json_free_serialized_string(json);
}
