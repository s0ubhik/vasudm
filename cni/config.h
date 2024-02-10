#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
    char *name;
    char *value;
} keyval;

typedef struct
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
} config_t;

#endif
