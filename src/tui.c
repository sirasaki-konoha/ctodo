#include "tui.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "json_read.h"
#include "json_write.h"
#include "kutil.h"
#include "read.h"

TodoJson load_todos()
{
    char* content = read_file("test/todo.json");
    TodoJson td = parse_todo((const char*)content);

    free(content);
    return td;
}

int add_todo() {

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

int main_loop()
{
    int selected = 0;
    TodoJson td = load_todos();

    while (1) {
        clear();
        mvprintw(0, 0, "add:  a  delete: d  quit: q");
        mvprintw(1, 0, "down: j  up:  k");

        draw_todos(&td, selected);

        refresh();

        int ch = getch();
        if (ch == 'q')
            break;
        if ((ch == KEY_UP || ch == 'k') && selected >= 1)
            selected--;
        if ((ch == KEY_DOWN || ch == 'j') && selected < td.len - 1)
            selected++;
        if (ch == 'a')
          add_todo();
    }

    free_todo(&td);
    return 0;
}

int enter_todos_tui()
{
    initscr();
    noecho();

    cbreak();
    keypad(stdscr, TRUE);

    main_loop();

    endwin();
    return 0;
}
