#include <stdlib.h>
#include <stdio.h>

void execute_script(const char *script) {// this will execute a bash or python script depending on the system

    char command[256];
    snprintf(command, sizeof(command), "python %s", script); // Adjust based on script type
    system(command);
}