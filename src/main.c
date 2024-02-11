#include <stdio.h>
#include "desktop.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "graphics.h"
#include "ui.h"
#include "auth.h"
#include "desktop.h"
#include "utils.h"


int main(int argc, char const *argv[])
{
    use_fb("fb0");
    read_config();
    cursor_off();
    echo_off();
    uidata_t* ui_data = get_ui();
    clear_screen();
    usleep(100); // wait for display driver
    render_ui();

    while (1)
    {
        clear_screen();

        // render ui
        render_ui();

        // get credentails
        get_input();

        // authenticate
        ui_message("Logging in...");
        if (auth_check(ui_data->username, ui_data->password) == 1) break;
    }

    // successfull login
    ui_success("Welcome");
    render_ui();
    auth_close();
    close_fb();
    cursor_on();
    echo_on();
    
    // switch user
    switch_user(ui_data->username);

    // start desktop
    // start_desktop(ui_data.desktop);
    execlp("/bin/bash", "/bin/bash", NULL);
    
    sleep(2);
    clear_screen();
}
