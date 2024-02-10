#ifndef UI_H
#define UI_H

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
    char username[50];
    char password[50];
} uidata_t;


void render_ui();

void get_input();

void render_inputs();

void move_next_input();
void tt();

#endif