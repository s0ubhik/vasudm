#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "graphics.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>


void get_terminal_size(int *rows, int *cols) {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}



int getch();

void get_input(char * dest){  
	int i=0;  
	while(1){  
		char c = getch();  
        printf("%d ", c);
        if (c == 27) { // ASCII code for Escape key
            // This might be an arrow key, check for the next two characters
            c = getch();
            if (c == 91) { // '[' following Escape indicates an arrow key
                c = getch();

                if (c == 65) {
                    printf("Up Arrow Pressed\n");
                } else if (c == 66) {
                    printf("Down Arrow Pressed\n");
                }
            }
        }
		if(c == '\n'){  
			dest[i] = 0;  
			break;  
		} else {  
			dest[i] = c;
            printf("%c",c);
		}  
		i++;  
	}  
}

void get_pswd(char * dest){  
	int i=0;  
	while(1){  
		char c = getch();  
        printf("%d ", c);
        if (c == 27) { // ASCII code for Escape key
            // This might be an arrow key, check for the next two characters
            c = getch();
            if (c == 91) { // '[' following Escape indicates an arrow key
                c = getch();

                if (c == 65) {
                    printf("Up Arrow Pressed\n");
                } else if (c == 66) {
                    printf("Down Arrow Pressed\n");
                }
            }
        }

		if(c == '\n'){  
			dest[i] = 0;  
			break;  
		} else {  
			dest[i] = c;
            printf("*");
		}  
		i++;  
	}  
}


int getch() {
   struct termios oldtc;
   struct termios newtc;
   int ch;
   tcgetattr(STDIN_FILENO, &oldtc);
   newtc = oldtc;
   newtc.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
   ch=getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
   return ch;
}

int main4() {
    uint32_t width = 200;
    uint32_t height = 30;
    uint32_t borderWidth = 2;
    uint32_t borderRadius = 10;
    uint32_t plborder = 0x383838;
    uint32_t acborder = 0x0F94D2;
    system("clear");
    int rows, cols;
    get_terminal_size(&rows, &cols);
    use_fb("fb0");

    draw_image(900, 390, "res/icons/arch.jpg");
    draw_rect_round(840, 507, width, height, borderWidth, borderRadius, acborder);
    draw_rect_round(840, 555, width, height, borderWidth, borderRadius, plborder);
    char in[256];

    // draw_rect(840, 505, width, height, borderWidth, 0x0F94D2);
    // draw_rect(840, 553, width, height, borderWidth, 0x383838);
    close_fb();
    move_cursor((507+32)/16, (840+16)/8);
    printf("login");

    move_cursor((555+32)/16, (840+16)/8);
    printf("password");

    move_cursor((507+32)/16, (840+16)/8);
    get_input(in);

    move_cursor((555+32)/16, (840+16)/8);
    get_pswd(in);

    // move_cursor(0, 201);
    // printf("F1 change session F2 shutdown F3 reboot ");


    // move_cursor(0, 0);
    // printf("Cinnamon Default");




    move_cursor(67, 0);
    while(1) {

    }


    return 0;
}
