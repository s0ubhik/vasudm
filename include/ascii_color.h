#ifndef COLORS_H
#define COLORS_H
#include "string.h"

typedef struct colors
{
    char *name;
    char *value;
} color_t;

static color_t colors[] = {
    { "BLACK"       ,"\e[0;30m"},
    { "RED"         ,"\e[0;31m"},
    { "GREEN"       ,"\e[0;32m"},
    { "YELLOW"      ,"\e[0;33m"},
    { "BLUE"        ,"\e[0;34m"},
    { "MAGENTA"     ,"\e[0;35m"},
    { "CYAN"        ,"\e[0;36m"},
    { "WHITE"       ,"\e[0;37m"},
    { "BOLD_BLACK"  ,"\e[1;30m"},
    { "BOLD_RED"    ,"\e[1;31m"},
    { "BOLD_GREEN"  ,"\e[1;32m"},
    { "BOLD_YELLOW" ,"\e[1;33m"},
    { "BOLD_BLUE"   ,"\e[1;34m"},
    { "BOLD_MAGENTA","\e[1;35m"},
    { "BOLD_CYAN"   ,"\e[1;36m"},
    { "BOLD_WHITE"  ,"\e[1;37m"},
};

static char* ascii_color(char *name) {
    for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
        if (strcmp(name, colors[i].name) == 0) {
            return colors[i].value;
        }
    }
    return "\e[0;37m";
}

#endif