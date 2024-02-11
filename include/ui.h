#ifndef UI_H
#define UI_H
#include <stdint.h>
typedef enum {
    INPUT_LOGIN,
    INPUT_PASWD,
    INPUT_END,
} uiselect_t;

typedef struct
{
    uiselect_t select;
    uint32_t x_inputs;
    uint32_t y_inputs;
    uint32_t scrn_cols;
    uint32_t scrn_rows;
    char username[50];
    char password[50];
    char* logColor;
    char* logText;
} uidata_t;


void render_ui();

void render_inputs();

void render_log();

void get_input();

void move_next_input();

void move_previous_input();

void ui_error(char *msg);

void ui_success(char *msg);

void ui_message(char *msg);

uidata_t* get_ui();

#endif