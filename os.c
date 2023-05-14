#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Iterate through command line arguments
    for (int i = 1; i < argc; i++) {
        // Check if the file has a .c extension
        char *extension = strrchr(argv[i], '.');
        if (extension != NULL && strcmp(extension, ".c") == 0) {
            // Execute the bash script passing the C file as argument
            char command[100];
            snprintf(command, sizeof(command), "./script.sh %s", argv[i]);
            system(command);
        } else {
            printf("Skipping %s: Not a C file\n", argv[i]);
        }
    }
    
    return 0;
}