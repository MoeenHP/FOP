#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int run_init(int argc, char * const argv[]) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;

    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }
        //Checking the existance of .neogit folder
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, cwd) != 0);// replaced "/" with cwd

    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;
        
    if (!exists) {
        if (mkdir(".neogit", 0755) != 0){
            printf("Error creating repo\n");
            return 1;
        }
        printf("Repo has successfully been created\n");
    } else {
        printf("neogit repository has already been initialized\n");
    }
    return 0;
}

int main (int argc, char *argv[]){
    if(argc < 2) {
        printf("Please enter a valid command\n");
        return 1;
    }
    if(strcmp(argv[1], "init") == 0){
        int result = run_init(argc, argv);
        if (result != 0) {
            printf("Repo initialization failed\n");
            return result;
        }
    }
    return 0;
}