#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "desktop.h"
#include "ini.h"

static desktops_t desktops = { .count = 0 };

int add_desktop(char *name, deskt_t type, char *path, char *exec)
{
    environ_t environ = {
        .name = name,
        .path = path,
        .type = type,
        .exec = exec,
    };

    desktops.environ[desktops.count] = environ;
    return desktops.count++;
}

int handle_desktop_entry(void* user, const char* section, const char* name, const char* value)
{
    if (strcmp(name, "Name") == 0) 
        desktops.environ[desktops.count-1].name = strdup(value);

    else if (strcmp(name, "Exec") == 0)
        desktops.environ[desktops.count-1].exec =  strdup(value);

    return 1;
}

void crawl_session_dir(char *path, deskt_t type)
{
    DIR *dir = opendir(path);
    if (dir == NULL) return;
    
    struct dirent *entry;

    while( (entry = readdir(dir)) != NULL ){
        if (entry->d_type != DT_REG || strstr(entry->d_name, ".desktop") == NULL) continue;

        char filename[MAX_PATH_LENGTH];
        snprintf(filename, sizeof(filename), "%s/%s", path, entry->d_name);

        add_desktop(NULL, type, strdup(filename), NULL);
        ini_parse(filename, handle_desktop_entry, NULL);
    }

    closedir(dir);
}

desktops_t* crawl_desktop()
{
    /* crawl for xorg sessions */
    crawl_session_dir("/usr/share/xsessions", XORG);

    /* crawl for wayland sessions */
    crawl_session_dir("/usr/share/wayland-sessions", WAYLAND);

    /* custom sessions */
    add_desktop("Shell", SHELL, NULL, NULL);

    return &desktops;
}

void start_shell(environ_t *env)
{

}

void start_xorg(environ_t *env)
{
  printf("Hiii %s\n", env->name);
}

void start_wayland(environ_t *env)
{

}

void start_desktop(int index)
{
    if (index < 0 || index > desktops.count) return;
    environ_t environ = desktops.environ[index];

    switch (environ.type){

    case WAYLAND:
        start_wayland(&environ);
        break;

    case XORG:
        start_xorg(&environ);
        break;

    case SHELL:
        start_shell(&environ);
        break;

    default:
        break;
    }
}
