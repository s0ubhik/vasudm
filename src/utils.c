#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void trim(char *str) 
{
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char)str[begin]))
        begin++;

    while (end >= begin && isspace((unsigned char)str[end]))
        end--;

    for (int i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[end - begin + 1] = '\0';
}

void get_terminal_size(int *rows, int *cols) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

int getch() {
   struct termios oldtc;
   struct termios newtc;
   int ch;
   tcgetattr(STDIN_FILENO, &oldtc);
   newtc = oldtc;
   newtc.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
   ch=getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
   return ch;
}

bool _verror(const char* func_name, char *error_msg)
{
    printf("ERROR (%s): %s\n", func_name, error_msg);
    exit(1);
    return false;
}