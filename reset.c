#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int run_reset(int argc, char *const argv[], int arg_to_be_rm) {
    DIR *dir = opendir(".neogit/staging");
    if (dir == NULL) {
        perror("Error opening staging directory");
        return 1;
    }

    struct dirent *entry;
    char destination_path[100] = "/home/seyyed/Deleted_because_reset/";
    char command[200] = "mv ";

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, argv[arg_to_be_rm]) == 0) {
            strcat(command, ".neogit/staging/");
            strcat(command, argv[arg_to_be_rm]);
            strcat(command, " ");
            strcat(command, destination_path);
            printf("%s\n", command);
            system(command);
            closedir(dir);
            return 0;
        }
    }
    
    closedir(dir);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please enter a valid command\n");
        return 1;
    }

    if (strcmp(argv[1], "reset") == 0) {
        if (strcmp(argv[2], "-f") == 0) {
            for (int i = 3; i < argc; i++) {
                int result = run_reset(argc, argv, i);
                if (result != 0) {
                    printf("Error unstaging input\n");
                    return result;
                }
            }
        } else {
            int result = run_reset(argc, argv, 2);
            if (result != 0) {
                printf("Error unstaging input\n");
                return result;
            }
        }
    }
    return 0;
}