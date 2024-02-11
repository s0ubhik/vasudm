#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

static struct termios old_term, new_term;

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

void echo_off()
{
    // disable echo
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void echo_on()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}

void cursor_on()
{
    printf("\e[?25h\n");
}

void cursor_off()
{
    printf("\e[?25l\n");
}

void move_cursor(int row, int col)
{
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

void clear_screen()
{
    printf("\033c\n");
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

int switch_user(char* username)
{
    struct passwd *pwd;
    int ok = 0;

    pwd = getpwnam(username);
    
    if (pwd == NULL) return 1;

    // set user info
    ok = initgroups(pwd->pw_name, pwd->pw_gid);
    if (ok != 0) return 2;

    // set gid
    ok = setgid(pwd->pw_gid);
    if (ok != 0) return 3;

    // set uid
    ok = setuid(pwd->pw_uid);
    if (ok != 0) return 4;

    return 0;
}
