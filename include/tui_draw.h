#ifndef _INTERNAL_TUI_DRAW_H
#define _INTERNAL_TUI_DRAW_H

char* dialog_input(const char* prompt);
int dialog_confirm(const char* message);
#define get_input(prompt) dialog_input(prompt)

#endif /* _INTERNAL_TUI_DRAW_H */
