#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#define verror(error_msg) _verror(__func__, error_msg)

bool _verror(const char *func_name, char *error_msg);

void trim(char *str);

void get_terminal_size(int *rows, int *cols);

int getch();

void echo_on();

void echo_off();

void cursor_on();

void cursor_off();

void move_cursor(int row, int col);

void clear_screen();

int switch_user(char* username);

#endif