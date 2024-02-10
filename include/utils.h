#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

#define verror(error_msg) _verror(__func__, error_msg)

bool _verror(const char *func_name, char *error_msg);

void trim(char *str);

void get_terminal_size(int *rows, int *cols);

int getch();
#endif