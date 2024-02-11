#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "graphics.h"
#include "utils.h"
#include "ui.h"

static uidata_t ui_data = { .select = INPUT_LOGIN };

void move_previous_input()
{
    if (ui_data.select == 0)
        ui_data.select = INPUT_END - 1;
    else ui_data.select -= 1;
}

void move_next_input()
{
    if (ui_data.select+1 == INPUT_END)
        ui_data.select = 0;
    else ui_data.select += 1;
}

void get_input(){
    config_t* config = get_config();
    char *dest;
	int i = 0;

    strncpy(ui_data.password, "", strlen(ui_data.password));

    if (config->username != NULL) {
        strcpy(ui_data.username, config->username);
        ui_data.select = INPUT_PASWD;
        config->username = NULL; // only once
    }


	while(1){
        render_inputs();
        if (ui_data.select ==  INPUT_LOGIN) dest = ui_data.username;
        else if (ui_data.select ==  INPUT_PASWD) dest = ui_data.password;
        i = strlen(dest);
        move_cursor((ui_data.y_inputs + config->spaceBetweenInput*ui_data.select)/16 + 2, (ui_data.x_inputs)/8 + 2 + i);

		char c = getch();

        // backspace
        if (c == 127) {
            dest[i-1] = '\0';
            continue;
        }

        // arrows
        if (c == 27) { 
            c = getch();
            if (c != 91) continue;
            c = getch();
            if (c == 65) {
                move_previous_input();
            } else if (c == 66) {
                move_next_input();
            }
            continue;
        }

        // enter
        if(c == '\n'){  
			dest[i] = 0;
            if (ui_data.select ==  INPUT_LOGIN) { move_next_input(); continue;}
            printf("\n\nusername: %s password: %s", ui_data.username, ui_data.password);
            break;
		}

        // allow only ascii
        if (c<32 || c>127) continue;

        // save charecter
        dest[i] = c;
		i++;
	}  
}


void render_inputs()
{
    config_t* config = get_config();

    /* print current values */
    move_cursor((ui_data.y_inputs)/16 + 2, (ui_data.x_inputs)/8 + 2);
    for(int i = 0; i < config->inputWidth/8; i++)
        printf(" ");
    move_cursor((ui_data.y_inputs + config->spaceBetweenInput)/16 + 2, (ui_data.x_inputs)/8 + 2);
    for(int i = 0; i < config->inputWidth/8; i++)
        printf(" ");

    move_cursor((ui_data.y_inputs)/16 + 2, (ui_data.x_inputs)/8 + 2);
    printf("%s",ui_data.username);
    move_cursor((ui_data.y_inputs + config->spaceBetweenInput)/16 + 2, (ui_data.x_inputs)/8 + 2);
    for(int i = 0; i < strlen(ui_data.password); i++)
        printf("%c",config->secureMask);

    /* custom adjustments please */
    if (config->borderRadius == 0)
    {
        draw_rect(ui_data.x_inputs-4,  ui_data.y_inputs-2, config->inputWidth, config->inputHeight, config->borderWidth,
            (ui_data.select ==  INPUT_LOGIN) ? config->accentColor : config->secondaryColor);
        draw_rect(ui_data.x_inputs-4,  ui_data.y_inputs -4 + config->spaceBetweenInput, config->inputWidth, config->inputHeight, config->borderWidth,
            (ui_data.select ==  INPUT_PASWD) ? config->accentColor : config->secondaryColor);
    } else {
        draw_rect_round(ui_data.x_inputs-4, ui_data.y_inputs-2, config->inputWidth, config->inputHeight, config->borderWidth, config->borderRadius,
            ui_data.select ==  INPUT_LOGIN ? config->accentColor : config->secondaryColor);
        draw_rect_round(ui_data.x_inputs-4, ui_data.y_inputs-4 + config->spaceBetweenInput, config->inputWidth, config->inputHeight, config->borderWidth, config->borderRadius,
            ui_data.select ==  INPUT_PASWD ? config->accentColor : config->secondaryColor);
    }
}


void render_ui()
{
    config_t* config = get_config();
    uint32_t screen_width, screen_height;

    get_fb_size(&screen_width, &screen_height);
    uint32_t voffset = (screen_height - (config->inputHeight*2 + config->spaceBetweenInput))/2 - (config->inputHeight*2 + config->spaceBetweenInput);

    if (config->header.name == NULL || (strcmp(config->header.name, "image") == 0)){
        /* Logo */
        uint32_t img_x, img_y;
        voffset = (uint32_t) (screen_height - (config->imageHeight + config->inputHeight*2 + config->spaceBetweenInput))/2; 
        
        if (config->imageX == -1) img_x = (int32_t) (screen_width-config->imageWidth)/2;
        else img_x = config->imageX;

        if (config->imageY == -1) img_y = voffset;
        else img_y = config->imageY;

        draw_image(img_x, img_y, config->imageWidth, config->imageHeight, config->header.value);
        voffset += config->imageHeight + 20;

    } else if (strcmp(config->header.name, "text") == 0) {
        /* Text */
        // TODO: NOT implement
        verror("Text Header not implemented");
        return;

    } else { /* No Header */ }

    ui_data.x_inputs = (screen_width-config->inputWidth)/2;
    ui_data.y_inputs = voffset;
    render_inputs();
}
