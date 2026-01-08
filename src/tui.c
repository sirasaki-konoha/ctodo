#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <form.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tui.h"
#include "tui_draw.h"
#include "json_read.h"
#include "json_write.h"
#include "kutil.h"
#include "read.h"
#include "write.h"
#include "utils.h"
#include "meta.h"



int add_todo(TodoJson* td) {
    char* input = get_input("Title");
    if (strcmp(input, "") == 0) {
        free(input);
        return 0;
    }

    char* time_str = get_time();


    srand(time(NULL));

    Todos todo = new_todo(input, time_str, 0, rand());
    todo_push(td, todo);

    TodoJson fmt_td = format_todos(*td);
    char* generated = create_json(fmt_td);

    Writer wt = w_create("test/todo.json");
    w_add_string(&wt, generated);
    w_flush_write(&wt);

    w_free_writer(&wt);
    free(time_str);
    free(generated);
    free(input);

    return 0;
}

void delete_todo(TodoJson* td, int selected) {
    if (remove_todo(td, selected) != 0) {
        return;
    }

    TodoJson fmt_td = format_todos(*td);
    char* generated = create_json(fmt_td);

    Writer wt = w_create("test/todo.json");
    w_add_string(&wt, generated);
    w_flush_write(&wt);


    w_free_writer(&wt);
    free(generated);
}

void mark_done(TodoJson* td, int selected) {
    td->todo[selected].done = !td->todo[selected].done;

    TodoJson fmt_td = format_todos(*td);
    char* generated = create_json(fmt_td);

    Writer wt = w_create("test/todo.json");
    w_add_string(&wt, generated);
    w_flush_write(&wt);


    w_free_writer(&wt);
    free(generated);
}

void draw_todos(TodoJson* raw, int selected, int scroll)
{
    TodoJson td = format_todos(*raw);
    int right_margin = COLS - 2;
    int list_top = 5;
    int list_height = LINES - list_top - 1;

    for (int row = 0; row < list_height; row++) {
        int idx = scroll + row;
        if (idx >= td.len)
            break;

        if (idx == selected)
            attron(A_REVERSE);

        if (td.todo[idx].done) {
            mvprintw(list_top + row, 0, "[");
            attron(COLOR_PAIR(2));
            mvprintw(list_top + row, 1, "X");
            attroff(COLOR_PAIR(2));
            mvprintw(list_top + row, 2, "]");

            mvprintw(list_top + row, 4, "%s", td.todo[idx].title);
        } else {
            mvprintw(list_top + row, 0, "[ ] %s", td.todo[idx].title);
        }

        const char* date = td.todo[idx].created_at;
        int x = right_margin - strlen(date);
        if (x < 0)
            x = 0;

        mvprintw(list_top + row, x, "(%s)", date);

        if (idx == selected)
            attroff(A_REVERSE);
    }
}

void draw_ui(TodoJson* td, int selected, int scroll) {
    mvprintw(0, 0, "Press 'h' to help");
    draw_todos(td, selected, scroll);
    mvprintw(LINES - 1, 0,
             "ctodo %s | built %s %s",
             VERSION,
             __DATE__,
             __TIME__);

}

void draw_msg(const char* msg) {
    attron(COLOR_PAIR(1));
    mvprintw(1, 0, "%s", msg);
    attroff(COLOR_PAIR(1));
}

int main_loop()
{
    int selected = 0;
    TodoJson td = load_todos();
    char* msg = format_string("");

    int scroll = 0;
    int list_top = 5;
    int list_height = LINES - list_top - 1;

    while (1) {
        clear();
        td = format_todos(td);
        draw_ui(&td, selected, scroll);
        draw_msg(msg);
        refresh();

        int ch = getch();
        if (ch == 'q')
            break;
        if ((ch == KEY_UP || ch == 'k') && selected >= 1) {
            if (selected > 0) {
                selected--;
                if (selected < scroll)
                    scroll--;
            }
        }
        if ((ch == KEY_DOWN || ch == 'j') && selected < td.len - 1){
            if (selected < td.len - 1) {
                selected++;
                if (selected >= scroll + list_height) {
                    scroll++;
                }
            }

        }
        if (ch == 'a')
          add_todo(&td);
        if (ch == 'd') {
            if (dialog_confirm("Delete todo?")) {
                delete_todo(&td, selected);
                if (selected >= td.len && td.len > 0) {
                    selected = td.len - 1;
                }
            } else {
                free(msg);
                msg = safe_strdup("Delete canceled.");
            }
        }
        if (ch == ' ')
            mark_done(&td, selected);

    }

    free_todo(&td);
    free(msg);
    return 0;
}

int enter_todos_tui()
{

    setlocale(LC_ALL, "");


    initscr();
    curs_set(0);
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_MAGENTA, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(4, COLOR_CYAN, -1);

    main_loop();

    endwin();
    return 0;
}
