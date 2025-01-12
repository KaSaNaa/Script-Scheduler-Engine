#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void execute_script(const char *script) { // this will execute a bash or python script depending on the system
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }

    if (access(script, F_OK) != -1){
        printf("Script file exists: %s\n", script);
    } else {
        printf("Script file doesnt not exist: %s\n", script);
        return;
    }

    char command[256];
    snprintf(command, sizeof(command), "python %s", script); // Adjust based on script type
    printf("Executing command: %s\n", command);
    system(command);
}