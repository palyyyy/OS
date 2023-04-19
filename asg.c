#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct stat st;

    // loop through all command-line arguments
    for (int i = 1; i < argc; i++) {
        printf("%s:\n", argv[i]);

        // get file info using stat() system call
        if (stat(argv[i], &st) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // print out file type and permissions
        printf("  File type:                ");

        switch (st.st_mode & S_IFMT) {
            case S_IFREG:
                printf("regular file\n");
                break;
            case S_IFDIR:
                printf("directory\n");
                break;
            case S_IFLNK:
                printf("symbolic link\n");
                break;
            default:
                printf("unknown file type\n");
                break;
        }

        printf("  Permissions:              %o\n", st.st_mode & 0777);
        printf("  Number of links:          %ld\n", (long) st.st_nlink);
        printf("  Owner ID:                 %ld\n", (long) st.st_uid);
        printf("  Group ID:                 %ld\n", (long) st.st_gid);
        printf("  Size in bytes:            %lld\n", (long long) st.st_size);
        printf("  Last access time:         %s", ctime(&st.st_atime));
        printf("  Last modification time:   %s", ctime(&st.st_mtime));
        printf("  Last status change time:  %s", ctime(&st.st_ctime));
    }

    return 0;
}
