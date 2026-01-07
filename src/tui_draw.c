
#include <curses.h>
#include <form.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "kutil.h"
#include "write.h"
#include "tui_draw.h"


void draw_dialog_box(WINDOW* win) {
    wattron(win, COLOR_PAIR(4));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(4));
}

char* dialog_input(const char* prompt) {
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

