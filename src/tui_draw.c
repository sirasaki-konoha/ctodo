
#include <curses.h>
#include <form.h>
#include <ncurses.h>
#include <string.h>

#include "kutil.h"
#include "tui_draw.h"

#define HEADER_H 3
#define FOOTER_H 3

void draw_dialog_box(WINDOW* win)
{
    wattron(win, COLOR_PAIR(4));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(4));
}

char* dialog_input(const char* prompt)
{
    int width = 40;
    int height = 5;

    WINDOW* win = newwin(height, width,
        (LINES - height) / 2,
        (COLS - width) / 2);

    draw_dialog_box(win);
    mvwprintw(win, 1, 2, "%s", prompt);
    wrefresh(win);

    echo();
    curs_set(1);

    char buf[256];
    mvwgetnstr(win, 2, 2, buf, 255);

    noecho();
    curs_set(0);

    delwin(win);
    return safe_strdup(buf);
}

int dialog_confirm(const char* message)
{
    timeout(-1);
    int width = strlen(message) + 10;
    int height = 5;

    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    WINDOW* win = newwin(height, width, starty, startx);
    draw_dialog_box(win);

    mvwprintw(win, 1, 2, "%s", message);
    mvwprintw(win, 2, 2, "[y] Yes   [n] No");

    wrefresh(win);

    int ch;
    while (1) {
        ch = getch();

        if (ch == 'y' || ch == 'Y') {
            delwin(win);
            return 1;
        } else {
            delwin(win);
            return 0;
        }
    }
}

WINDOW* create_header()
{
    int height = HEADER_H;
    int width = COLS;

    int starty = 0;
    int startx = 0;

    WINDOW* win = newwin(height, width, starty, startx);

    return win;
}

WINDOW* create_todo_list_win()
{
    int height = LINES - HEADER_H - FOOTER_H;
    int width = (COLS / 2) - 1;

    int starty = HEADER_H;
    int startx = (COLS / 2) / (COLS / 2);

    WINDOW* win = newwin(height, width, starty, startx);

    return win;
}

WINDOW* create_selected_win()
{
    int height = LINES - HEADER_H - FOOTER_H;
    int width = COLS / 2;

    int starty = HEADER_H;
    int startx = COLS / 2;

    WINDOW* win = newwin(height, width, starty, startx);

    return win;
}

WINDOW* create_footer()
{
    int screen_h, screen_w;
    getmaxyx(stdscr, screen_h, screen_w);

    int header_y = 0;
    int content_y = HEADER_H;
    int content_h = screen_h - HEADER_H - FOOTER_H;
    int footer_y = HEADER_H + content_h;

    WINDOW* win = newwin(FOOTER_H, screen_w, footer_y, 0);

    return win;
}
