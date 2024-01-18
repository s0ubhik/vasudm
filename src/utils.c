#include "utils.h"
#include <ctype.h>
#include <string.h>

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