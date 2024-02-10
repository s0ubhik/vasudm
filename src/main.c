#include <stdio.h>
#include "desktop.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"
#include "config.h"
#include "graphics.h"
#include "ui.h"


/*
typedef struct
{
    char *name;
    char *value;
} keyval;


static struct 
{
    char *username;

    char secureMask;
    keyval session;
    keyval header; 

    bool showHelp;
    bool showSession;

    uint8_t borderWidth;
    uint8_t borderRadius;

    uint32_t imageX;
    uint32_t imageY;

    uint32_t accentColor;
    uint32_t headerColor;
    uint32_t textColor;
    uint32_t borderInactiveColor;
    uint32_t placeHolderColor;
} config;


void parse_bool(const char *value, bool *item)
{
    *item = (bool)( strcmp(value, "true") == 0 );
}


void parse_subpair(const char *value, keyval *item)
{
    char name[256]; char val[256];
    sscanf(value, "%[^:] : %[^\n]",name,val);
    trim(name); trim(val);
    item->name = strdup(name);
    item->value = strdup(val);
}

int handle_config(void* user, const char* section, const char* name, const char* value)
{
    if (strcmp(name, "defaultUsername") == 0) 
        config.username = strdup(value);

    else if (strcmp(name, "header") == 0) 
        parse_subpair(value, &config.header);

    else if (strcmp(name, "defaultSession") == 0) 
        parse_subpair(value, &config.header);

    else if (strcmp(name, "showHelp") == 0) 
        parse_bool(value, &config.showHelp);

    else if (strcmp(name, "showSession") == 0) 
        parse_bool(value, &config.showSession);

    else if (strcmp(name, "borderWidth") == 0) 
        config.borderWidth = atoi(value);

    else if (strcmp(name, "borderRadius") == 0) 
        config.borderRadius = atoi(value);

    else if (strcmp(name, "imageX") == 0) 
        config.imageX = atoi(value);

    else if (strcmp(name, "imageY") == 0) 
        config.imageY = atoi(value);

    return 1;
}
*/

int main(int argc, char const *argv[])
{
    use_fb("fb0");
    system("clear");

    read_config();
    char s[10];
    
    while (1){
        render_ui();
        get_input();
    }

    close_fb();
}

/*
int main_main()
{
    char username[256];
    int8_t environ_index;

    load_config();

    crawl_desktop();

    while (1) {
        ui_render();

        if (handle_keybaoard() == 0) continue;

        if (auth_pam() == 0) {
            ui_log(LOG_SUCCESS, "Welcome");
            break;
        }

        ui_log(LOG_ERROR, "Invalid Login");
    }

    switch_user(username);

    start_session(environ_index);
}

*/
