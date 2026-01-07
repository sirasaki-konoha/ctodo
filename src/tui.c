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
    char* input = get_input("Title: ");
    char* time = get_time();


    Todos todo = new_todo(input, time, 0);
    todo_push(td, todo);

    char* generated = create_json(*td);

    Writer wt = w_create("test/todo.json");
    w_add_string(&wt, generated);
    w_flush_write(&wt);

    w_free_writer(&wt);
    free(time);
    free(generated);
    free(input);

    return 0;
}

void delete_todo(TodoJson* td, int selected) {
    if (remove_todo(td, selected) != 0) {
        return;
    }

    char* generated = create_json(*td);

    Writer wt = w_create("test/todo.json");
    w_add_string(&wt, generated);
    w_flush_write(&wt);


    w_free_writer(&wt);
    free(generated);
}

void draw_todos(TodoJson* td, int selected)
{
    int right_margin = COLS - 2;

    for (int i = 0; i < td->len; i++) {
        if (i == selected)
            attron(A_REVERSE);

        mvprintw(i + 5, 0, "[%c] %s", td->todo[i].done ? 'x' : ' ',
            td->todo[i].title);

        const char* date = td->todo[i].created_at;
        int x = right_margin - strlen(date);
        if (x < 0)
            x = 0;

        mvprintw(i + 5, x, "(%s)", date);

        if (i == selected)
            attroff(A_REVERSE);
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
            char* input = get_input("delete todo? ");
            if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) {
                delete_todo(&td, selected);
                if (selected >= td.len && td.len > 0) {
                    selected = td.len - 1;
                }
            } else {
                free(msg);
                msg = safe_strdup("Delete canceled.");
            }

            free(input);
        }

    }

    free_todo(&td);
    free(msg);
    return 0;
}

int enter_todos_tui()
{

    setlocale(LC_ALL, "");


    initscr();
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_MAGENTA, -1);
    init_pair(4, COLOR_CYAN, -1);

    main_loop();

    endwin();
    return 0;
}
