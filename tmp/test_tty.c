#include <stdio.h>
#include <stdlib.h>

int is_terminal() {
    return getenv("TTY") != NULL;
}

int main() {
    if (is_terminal()) {
        printf("The program is running in a terminal.\n");
    } else {
        printf("The program is not running in a terminal.\n");
    }

    return 0;
}