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

void draw_todos(TodoJson* raw, int selected)
{
    TodoJson td = format_todos(*raw);
    int right_margin = COLS - 2;
    int row = 5;



    for (int i = 0; i < td.len; i++) {
            if (i == selected)
                attron(A_REVERSE);

        if (td.todo[i].done) {
            mvprintw(row, 0, "[");
            attron(COLOR_PAIR(2));
            mvprintw(row, 1, "X");
            attroff(COLOR_PAIR(2));
            mvprintw(row, 2, "]");

            mvprintw(row, 4, "%s", td.todo[i].title);
        } else {
            mvprintw(row, 0, "[ ] %s", td.todo[i].title);
        }

        const char* date = td.todo[i].created_at;
        int x = right_margin - strlen(date);
        if (x < 0)
            x = 0;

        mvprintw(row, x, "(%s)", date);

        if (i == selected)
            attroff(A_REVERSE);
        row++;

    }
}

void draw_ui(TodoJson* td, int selected) {
    mvprintw(0, 0, "add:  a  delete: d  quit: q");
    mvprintw(1, 0, "down: j  up:  k");
    draw_todos(td, selected);

}

void draw_msg(const char* msg) {
    attron(COLOR_PAIR(1));
    mvprintw(3, 0, "%s", msg);
    attroff(COLOR_PAIR(1));
}

int main_loop()
{
    int selected = 0;
    TodoJson td = load_todos();
    char* msg = format_string("");

    while (1) {
        clear();
        td = format_todos(td);
        draw_ui(&td, selected);
        draw_msg(msg);
        refresh();

        int ch = getch();
        if (ch == 'q')
            break;
        if ((ch == KEY_UP || ch == 'k') && selected >= 1)
            selected--;
        if ((ch == KEY_DOWN || ch == 'j') && selected < td.len - 1)
            selected++;
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
