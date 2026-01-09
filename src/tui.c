#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <form.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tui.h"
#include "tui_draw.h"
#include "json_read.h"
#include "json_write.h"
#include "kutil.h"
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

void draw_todo_list(WINDOW* win, TodoJson* raw, int selected, int scroll)
{
    TodoJson td = format_todos(*raw);
    int list_height = getmaxy(win);
    int list_top = 1;

    for (int row = 0; row < list_height; row++) {
        int idx = scroll + row;
        if (idx >= td.len)
            break;

        if (idx == selected)
            wattron(win, A_REVERSE);

        if (td.todo[idx].done) {
            mvwprintw(win, list_top + row, 1, "[");
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, list_top + row, 2, "X");
            wattroff(win, COLOR_PAIR(2));
            mvwprintw(win, list_top + row, 3, "]");

            mvwprintw(win, list_top + row, 5, "%s", td.todo[idx].title);
        } else {
            mvwprintw(win, list_top + row, 1, "[ ] %s", td.todo[idx].title);
        }

        if (idx == selected)
            wattroff(win, A_REVERSE);
    }

    draw_dialog_box(win);

    wrefresh(win);
}

void draw_footer(WINDOW* win) {
    mvwprintw(win, 1, 0,
             "ctodo %s | built %s %s",
             VERSION,
             __DATE__,
             __TIME__);
    draw_dialog_box(win);
}

void draw_header(WINDOW* win) {
    mvwprintw(win, 1, 1, "Press 'h' to help");
    draw_dialog_box(win);
    wrefresh(win);
}

void draw_msg(const char* msg) {
    attron(COLOR_PAIR(1));
    mvprintw(1, 0, "%s", msg);
    attroff(COLOR_PAIR(1));
}

void draw_detail(WINDOW* detail_win, TodoJson td, int selected) {
    mvwprintw(detail_win, 1, 1, "%s", td.todo[selected].title);
    mvwprintw(detail_win, 3, 1, "Created at %s, id: %d", td.todo[selected].created_at, td.todo[selected].id);
    if (td.todo[selected].done) {
        mvwprintw(detail_win, 5, 1, "Marked as ");
        wattron(detail_win, COLOR_PAIR(2));
        mvwprintw(detail_win, 5, 11, "DONE");
        wattroff(detail_win, COLOR_PAIR(2));
    }

    draw_dialog_box(detail_win);
    wrefresh(detail_win);
}

void check_xy() {

    if (LINES <= 40 || COLS <= 40) {
        char* format = format_string("Window width or height is too small! (current: width: %d height: %d;  expected: width: 30 height: 30)", COLS, LINES);
        dialog_confirm(format);
        free(format);
    }
}

int main_loop()
{
    int selected = 0;
    TodoJson td = load_todos();
    char* msg = format_string("");

    WINDOW* detail = create_selected_win();
    WINDOW* todo_list = create_todo_list_win();
    WINDOW* header = create_header();
    WINDOW* footer = create_footer();

    int scroll = 0;
    int list_top = 5;
    int list_height = LINES - list_top - 1;

    while (1) {
        td = format_todos(td);
        check_xy();


        werase(detail);
        werase(todo_list);
        werase(header);
        werase(footer);

        draw_header(header);
        draw_detail(detail, td, selected);
        draw_todo_list(todo_list, &td, selected, scroll);
        draw_footer(footer);
        draw_msg(msg);

        wrefresh(header);
        wrefresh(detail);
        wrefresh(todo_list);
        wrefresh(footer);
        refresh();

        int ch = getch();
        timeout(100);

        if (ch != ERR) {
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
            if (ch == 'a') {
                add_todo(&td);
            }
            if (ch == 'd') {
                if (dialog_confirm("Delete todo?")) {
                    delete_todo(&td, selected);
                    if (selected >= td.len && td.len > 0) {
                        selected = td.len - 1;
                    }
                }
            }
            if (ch == ' ') {
                mark_done(&td, selected);
            }
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
    curs_set(0);
    noecho();
    cbreak();
    start_color();
    use_default_colors();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    init_pair(1, COLOR_MAGENTA, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(4, COLOR_CYAN, -1);

    main_loop();

    endwin();
    return 0;
}
