#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "argtable3.h"
#include "json_read.h"
#include "json_write.h"
#include "meta.h"
#include "read.h"
#include "tui.h"
#include "write.h"

void display_todo()
{
    if (enter_todos_tui() != 0) {
        endwin();
        return;
    }
}

int main(int argc, char* argv[])
{
    struct arg_str* arg_todo = arg_str0("t", "todo", "TODO", "add specified todo");
    struct arg_lit* version = arg_lit0("v", "version", "display ctodo's version");
    struct arg_lit* help = arg_lit0("h", "help", "show help");
    struct arg_end* end = arg_end(10);
    void* argtable[] = { arg_todo, version, help, end };

    int nerrors = arg_parse(argc, argv, argtable);
    if (nerrors > 0) {
        arg_print_errors(stderr, end, argv[0]);
        printf("\n");
        arg_print_syntax(stdout, argtable, "\n");

        goto cleanup;
    }

    if (help->count > 0) {
        printf("Usage: ctodo");
        arg_print_syntax(stdout, argtable, "\n");
        printf("\nOptions\n");
        arg_print_glossary(stdout, argtable, "    %-25s %s\n");
        goto cleanup;
    }

    if (version->count > 0) {
        printf(
            "ctodo version %s\n"
            "Compiled at %s %s\n"
            "Licensed under MIT Licese. \n"
            "Source code available on https://github.com/sirasaki-konoha/ctodo\n",
            VERSION,
            __DATE__,
            __TIME__);
        goto cleanup;
    }

    if (!(arg_todo->count > 0)) {
        display_todo();
        goto cleanup;
    }

    goto cleanup;

cleanup:
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[2]));
    return 0;
}

// int main() {
//     char* content = read_file("test/todo.json");
//     TodoJson td = parse_todo((const char*)content);
//
//     for (int i = 0; i < td.len; i++) {
//         printf("content: title: %s, create_at: %s, done: %d\n",
//         td.todo[i].title, td.todo[i].created_at, td.todo[i].done);
//     }
//
//     Todos ts = new_todo("Work with dr. pepper", "2026-01-07", 0);
//
//     todo_push(&td, ts);
//
//     char* generated = create_json(td);
//
//     Writer wt = wopen("test/todo.json");
//     add_string(&wt, generated);
//     flush_write(&wt);
//
//
//     free_writer(&wt);
//     free(content);
//     free_json(generated);
//     free_todo(&td);
// }
//
