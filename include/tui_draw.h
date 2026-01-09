#ifndef _INTERNAL_TUI_DRAW_H
#define _INTERNAL_TUI_DRAW_H
#include <ncurses.h>

void draw_dialog_box(WINDOW* win);
char* dialog_input(const char* prompt);
int dialog_confirm(const char* message);
#define get_input(prompt) dialog_input(prompt)


WINDOW* create_header();
WINDOW* create_todo_list_win();
WINDOW* create_selected_win();
WINDOW* create_footer();

#endif /* _INTERNAL_TUI_DRAW_H */
