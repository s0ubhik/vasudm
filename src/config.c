#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "config.h"
#include "vendor/ini.h"
#include "utils.h"

static config_t config = {
    .imageX = -1,
    .imageY = -1,
    .imageWidth = 100,
    .imageHeight = 100,
    .borderWidth = 2,
    .borderRadius = 10,
    .inputWidth = 200,
    .inputHeight = 30,
    .accentColor = 0x0F94D2,
    .secondaryColor = 0x383838,
    .spaceBetweenInput = 50,
    .header = {.value = "res/icons/arch.jpg"}
};

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

    else if (strcmp(name, "imageWidth") == 0) 
        config.imageWidth = atoi(value);

    else if (strcmp(name, "imageHeight") == 0) 
        config.imageHeight = atoi(value);

    else if (strcmp(name, "inputWidth") == 0) 
        config.inputWidth = atoi(value);

    else if (strcmp(name, "inputHeight") == 0) 
        config.inputHeight = atoi(value);

    // else if (strcmp(name, "accentColor") == 0) 
    //     config.accentColor = atoi(value);

    // else if (strcmp(name, "imageHeight") == 0) 
    //     config.secondaryColor = atoi(value);

    else if (strcmp(name, "spaceBetweenInput") == 0) 
        config.spaceBetweenInput = atoi(value);


    return 1;
}

void read_config()
{
  ini_parse("res/vdm.config", handle_config, NULL);
}

config_t* get_config()
{
    return &config;
}

