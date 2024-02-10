#ifndef DESKTOP_H
#define DESKTOP_H

#define MAX_ENVIRONMENTS 10
#define MAX_PATH_LENGTH 256

typedef enum
{
    WAYLAND,
    XORG,
    SHELL
} deskt_t;

typedef struct
{
    char *name;
    char *path;
    deskt_t type;
    char *exec;
} environ_t;

typedef struct
{
    environ_t environ[MAX_ENVIRONMENTS];
    int count;
} desktops_t;

#endif
