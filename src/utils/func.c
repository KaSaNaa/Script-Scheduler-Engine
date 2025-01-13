#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void execute_script(const char *script) { // this will execute a bash or python script depending on the system
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // printf("Current working dir: %s\n", cwd);
        printf("\n");
    } else {
        perror("getcwd() error");
    }

    if (access(script, F_OK) != -1){
        // printf("Script file exists: %s\n", script);
        printf("\n");
    } else {
        // printf("Script file doesnt not exist: %s\n", script);
        printf("\n");
        return;
    }

    char command[256];

    const char *ext = strrchr(script, '.');
    
    if (ext != NULL) {
        if (strcmp(ext, ".sh") == 0) {
            snprintf(command, sizeof(command), "bash %s", script);
        } else if (strcmp(ext, ".py") == 0) {
            snprintf(command, sizeof(command), "python %s", script);
        } else {
            printf("Unsupported script type: %s\n", ext);
            return;
        }
    } else {
        printf("No file extension found in script: %s\n", script);
        return;
    }
    printf("Executing command: %s\n", command);
    system(command);
}