#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

// Function to check the type of a file
void checkFileType(const char *path) {
    struct stat fileStat;

    if (lstat(path, &fileStat) == -1) {
        perror("Error getting file information");
        return;
    }

    if (S_ISREG(fileStat.st_mode)) {
        // Regular file menu
        printf("Regular File Menu:\n");
        printf("1. Name (-n)\n");
        printf("2. Size (-d)\n");
        printf("3. Hard Link Count (-h)\n");
        printf("4. Time of Last Modification (-m)\n");
        printf("5. Access Rights (-a)\n\n");
        printf("Enter your choice (1-5): ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: %s\n", path);
                break;
            case 2:
                printf("Size: %ld bytes\n", fileStat.st_size);
                break;
            case 3:
                printf("Hard Link Count: %ld\n", fileStat.st_nlink);
                break;
            case 4:
                printf("Time of Last Modification: %s", ctime(&fileStat.st_mtime));
                break;
            case 5:
                printf("Access Rights: %o\n", fileStat.st_mode & 0777);
                break;
            default:
                printf("Invalid choice\n");
        }
    } else if (S_ISLNK(fileStat.st_mode)) {
        // Symbolic link menu
        printf("Symbolic Link Menu:\n");
        printf("1. Name (-n)\n");
        printf("2. Delete Symbolic Link (-l)\n");
        printf("3. Size of Symbolic Link (-d)\n");
        printf("4. Size of Target File (-t)\n");
        printf("5. Access Rights (-a)\n\n");
        printf("Enter your choice (1-5): ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: %s\n", path);
                break;
            case 2:
                if (unlink(path) == -1)
                    perror("Error deleting symbolic link");
                else
                    printf("Symbolic link deleted successfully\n");
                break;
            case 3:
                printf("Size of Symbolic Link: %ld bytes\n", fileStat.st_size);
                break;
            case 4:
                if (readlink(path, NULL, 0) == -1)
                    perror("Error getting target file size");
                else
                    printf("Size of Target File: %ld bytes\n", fileStat.st_size);
                break;
            case 5:
                printf("Access Rights: %o\n", fileStat.st_mode & 0777);
                break;
            default:
                printf("Invalid choice\n");
        }
    } else if (S_ISDIR(fileStat.st_mode)) {
        // Directory menu
        printf("Directory Menu:\n");
        printf("1. Name (-n)\n");
        printf("2. Size (-d)\n");
        printf("3. Access Rights (-a)\n");
        printf("4. Total number of files with .c extension (-c)\n\n");
        printf("Enter your choice (1-4): ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Name: %s\n", path);
                break;
            case 2:
                printf("Size: %ld bytes\n", fileStat.st_size);
                break;
            case 3:
                printf("Access Rights: %o\n", fileStat.st_mode & 0777);
                break;
            case 4:
                countFilesWithExtension(path, ".c");
                break;
            default:
                printf("Invalid choice\n");
        }
    } else {
        printf("%s is an unknown file type\n", path);
    }
}

// Function to count the number of files with a specific extension in a directory
void countFilesWithExtension(const char *dirPath, const char *extension) {
    DIR *dir = opendir(dirPath);
    struct dirent *entry;
    int count = 0;

    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, extension) != NULL)
            count++;
    }

    closedir(dir);

    printf("Total number of files with %s extension: %d\n", extension, count);
}

// Function to run a Bash script for files ending in ".c"
void runBashScript(const char *filePath) {
    if (strlen(filePath) > 2 && strcmp(filePath + strlen(filePath) - 2, ".c") == 0) {
        pid_t childPid = fork();

        if (childPid == 0) {
            // Child process
            execlp("bash", "bash", "script.sh", filePath, NULL);

            // This line will be reached only if execlp fails
            perror("Error executing Bash script");
            exit(1);
        } else if (childPid > 0) {
            // Parent process
            wait(NULL);
        } else {
            // Fork failed
            perror("Fork failed");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    pid_t childPid;
    int status;

    // Iterate through each argument
    for (int i = 1; i < argc; i++) {
        childPid = fork();

        if (childPid == 0) {
            // Child process

            // Check if the file ends in ".c" and run the Bash script if true
            runBashScript(argv[i]);

            // Check the type of the file
            checkFileType(argv[i]);

            exit(0);
        } else if (childPid > 0) {
            // Parent process
            printf("Started child process for %s\n", argv[i]);
            printf("\n");
        } else {
            // Fork failed
            perror("Fork failed");
            exit(1);
        }
    }

    // Wait for each child process to complete
    while (wait(&status) > 0) {}

    return 0;
}
