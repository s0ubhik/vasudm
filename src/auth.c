#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ui.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <security/pam_appl.h>

static pam_handle_t *pamh = NULL;

int my_pam_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
    *resp = calloc(num_msg, sizeof(struct pam_response));

    if (*resp == NULL) {
        return PAM_BUF_ERR;
    }

    char *username;
    char *password;
    int ok = PAM_SUCCESS;
    int i;

    for (i = 0; i < num_msg; ++i) {
        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_ON: {
                username = ((char **)appdata_ptr)[0];
                (*resp)[i].resp = strdup(username);
                break;
            }
            case PAM_PROMPT_ECHO_OFF: {
                password = ((char **)appdata_ptr)[1];
                (*resp)[i].resp = strdup(password);
                break;
            }
            case PAM_ERROR_MSG: {
                ok = PAM_CONV_ERR;
                break;
            }
        }

        if (ok != PAM_SUCCESS) {
            break;
        }
    }

    if (ok != PAM_SUCCESS) {
        for (i = 0; i < num_msg; ++i) {
            if ((*resp)[i].resp == NULL) {
                continue;
            }

            free((*resp)[i].resp);
            (*resp)[i].resp = NULL;
        }

        free(*resp);
        *resp = NULL;
    }

    return ok;
}

int auth_check(char* username, char* password)
{
    int pam_err;
    move_cursor(0, 0);

    char *creds[2] = {username, password};

    struct pam_conv pam_conversation = {
        my_pam_conversation,
        creds
    };

    // start
    pam_err = pam_start("mydm", username, &pam_conversation, &pamh);
    if (pam_err != PAM_SUCCESS)
    {
        ui_error("FAILED: PAM init");
        return -1;
    }

    // authenticate
    pam_err = pam_authenticate(pamh, PAM_DISALLOW_NULL_AUTHTOK);
    if (pam_err != PAM_SUCCESS)
    {
        ui_error("Incorrect Credentials");
        pam_end(pamh, pam_err);
        return -1;
    }

    // open session
    pam_err = pam_open_session(pamh, 0);
    if (pam_err != PAM_SUCCESS)
    {
        ui_error("FAILED: openning session");
        pam_end(pamh, pam_err);
        return -1;
    }        

    return 1;
}

void auth_close()
{
    int pam_err;
    // close session
    pam_close_session(pamh, 0);
    pam_end(pamh, pam_err);
}