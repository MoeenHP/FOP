#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int check_existance_repo() {
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        return 1;
    }

    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening current directory");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
            closedir(dir);
            return 0;
        }
    }
    closedir(dir);
    return 1;
}

int exist_in_stage (char *name) {
    DIR *dir = opendir(".neogit/staging");
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(name, entry->d_name) == 0) return 0;
    }
    return 1;
}
void dash_n () {
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    DIR *dir = opendir(cwd);
    if(dir == NULL) printf("Error opening current work directory\n");
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        printf("%s: ", entry->d_name);
        int result = exist_in_stage(entry->d_name);
        if(result == 0) printf("File is staged\n");
        else printf("Not staged\n");
    }
    closedir(dir);
}

void copy_system(char *source) {
    char command[100] = "cp -r ";
    strcat(command, source);
    strcat(command, " .neogit/staging/");
    system(command);
    printf("%s\n", command);
}

int add_to_staging(char *filepath) {
    copy_system(filepath);
    return 0;
}

int run_add(int argc, char *const argv[], int arg_to_be_add) {
    if (argc < 3) {
        printf("Please specify a file\n");
        return 1;
    }
    
    int result = check_existance_repo();
    if (result != 0) {
        printf("Please create a repo first\n");
        return 1;
    }
    
    return add_to_staging(argv[arg_to_be_add]);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please enter a valid command\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0) {
        if (strcmp(argv[2], "-f") == 0) {
            for (int i = 3; i < argc; i++) {
                int result = run_add(argc, argv, i);
                if (result != 0) {
                    printf("Failed to add file to staging\n");
                    return result;
                }
            }
        } else if (strcmp(argv[2], "-n") == 0) {
            dash_n();
            return 0;
        }
        else {
            int result = run_add(argc, argv, 2);
            if (result != 0) {
                printf("Failed to add file to staging\n");
                return result;
            }
        }
    }
    
    return 0;
}
