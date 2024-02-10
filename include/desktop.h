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

int add_desktop(char *name, deskt_t type, char *path, char *exec);

int handle_desktop_entry(void* user, const char* section, const char* name, const char* value);

void crawl_session_dir(char *path, deskt_t type);

desktops_t* crawl_desktop();

void start_desktop(int index);

void start_shell(environ_t *env);

void start_xorg(environ_t *env);

void start_wayland(environ_t *env);

#endif
