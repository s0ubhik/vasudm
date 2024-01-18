#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_ENVIRONMENTS 10

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    char command[MAX_FILENAME_LENGTH];
} DesktopEnvironment;

typedef struct {
    DesktopEnvironment environments[MAX_ENVIRONMENTS];
    int count;
} EnvironmentList;

EnvironmentList detectEnvironments(const char *sessionsPath) {
    EnvironmentList envList = { .count = 0 };

    DIR *dir;
    struct dirent *entry;

    dir = opendir(sessionsPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return envList;
    }

    while ((entry = readdir(dir)) != NULL && envList.count < MAX_ENVIRONMENTS) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".desktop") != NULL) {
            char filename[MAX_FILENAME_LENGTH];
            snprintf(filename, sizeof(filename), "%s/%s", sessionsPath, entry->d_name);

            FILE *file = fopen(filename, "r");
            if (file != NULL) {
                char line[256];
                DesktopEnvironment env;

                while (fgets(line, sizeof(line), file)) {
                    if (strstr(line, "Name=") != NULL) {
                        strcpy(env.name, strchr(line, '=') + 1);
                    } else if (strstr(line, "Exec=") != NULL) {
                        strcpy(env.command, strchr(line, '=') + 1);
                    }
                }

                fclose(file);

                // Remove trailing newline characters
                strtok(env.name, "\n");
                strtok(env.command, "\n");

                envList.environments[envList.count++] = env;
            }
        }
    }

    closedir(dir);

    return envList;
}

void startEnvironment(const char *command) {
    printf("Starting environment: %s\n", command);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking process");
    } else if (pid == 0) {
        // This is the child process
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        // If execl fails
        perror("Error executing command");
        _exit(EXIT_FAILURE);
    } else {
        // This is the parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exitStatus = WEXITSTATUS(status);
            printf("Environment exited with status: %d\n", exitStatus);
        } else if (WIFSIGNALED(status)) {
            int signalNumber = WTERMSIG(status);
            printf("Environment terminated by signal: %d\n", signalNumber);
        } else {
            printf("Environment exited abnormally\n");
        }
    }
}
void startX11Session() {
    system("startx");
}

int main2() {
    EnvironmentList waylandEnvironments = detectEnvironments("/usr/share/wayland-sessions");
    EnvironmentList x11Environments = detectEnvironments("/usr/share/xsessions");

    printf("Detected Wayland environments:\n");
    for (int i = 0; i < waylandEnvironments.count; ++i) {
        printf("- %s %s\n", waylandEnvironments.environments[i].name, waylandEnvironments.environments[i].command);
    }

    printf("\nDetected X11 environments:\n");
    for (int i = 0; i < x11Environments.count; ++i) {
        printf("- %s %s\n",x11Environments.environments[i].name,x11Environments.environments[i].command);

    }

    // Example: Start the first Wayland environment
    // if (waylandEnvironments.count > 0) {
    //     startEnvironment(waylandEnvironments.environments[0].command);
    // }

    // // Example: Start the first X11 environment
    // if (x11Environments.count > 0) {
    //     startEnvironment(x11Environments.environments[1].command);
    // }
startX11Session();
    return 0;
}




void startX11SessionAsUser(const char *username) {
    char command[256];
    snprintf(command, sizeof(command), "startx -- :7");
    system(command);
}

int isX11Session() {
    return (getenv("DISPLAY") != NULL);
}

int main3() {
    const char *targetUser = "soubhik";  // Replace with the desired username
        startX11SessionAsUser(targetUser);

    // if (isX11Session()) {
    //     printf("Starting X11 session with startx as user: %s\n", targetUser);
    // } else {
    //     printf("Not an X11 session. You might be using Wayland or another display server.\n");
    // }

    return 0;
}
