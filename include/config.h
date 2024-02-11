#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

typedef struct
{
    char *name;
    char *value;
} keyval;

typedef struct
{
    char *username;
    char *textColor;
    char secureMask;
    keyval session;
    keyval header;

    bool showHelp;
    bool showSession;

    uint8_t borderWidth;
    uint8_t borderRadius;
    
    int32_t imageX;
    int32_t imageY;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t inputWidth;
    uint32_t inputHeight;
    uint32_t accentColor;
    uint32_t headerColor;
    uint32_t borderInactiveColor;
    uint32_t secondaryColor;
    uint32_t spaceBetweenInput;

} config_t;

void parse_bool(const char *value, bool *item);

void parse_subpair(const char *value, keyval *item);

int handle_config(void* user, const char* section, const char* name, const char* value);

void read_config();

config_t* get_config();

#endif
