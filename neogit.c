#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

//Not good comments:
//The run_alias function doesnt work
//in run_add function "neogit add" cause segmentation fault
//

//Functions
int run_init(int argc, char * const argv[]);
int config_username(char *username, int argc, char *argv[]);
int config_email(char *email, int argc, char *argv[]);
int create_alias (int argc, char *argv[]);
int run_alias (int argc, char *argv[]);
int check_existance_repo();
int exist_in_stage (char *name);
void dash_n ();
void copy_system(char *source);
int add_to_staging(char *filepath);
int run_add(int argc, char *argv[], int arg_to_be_add);
int run_reset(int argc, char *const argv[], int arg_to_be_rm);
int create_commit (int argc, char *argv[]);
int run_commit(int argc, char *argv[], char *commitInfoFileName);
int parse_date (char *date_str, struct tm *date);

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

    } while (strcmp(tmp_cwd, cwd) != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;
        
    if (!exists) {
        if (mkdir(".neogit", 0755) != 0) return 1;
        if (chdir(".neogit") != 0) return 1;
        if (mkdir("staging", 0755) != 0) return 1;
        if (mkdir("commit", 0755) != 0) return 1;
        if (mkdir("files", 0755) != 0) return 1;
        FILE *file = fopen("local_config.txt", "w");
        if (chdir("commit") != 0) return 1;
        FILE *file2 = fopen("commit_names.txt", "w");
        fprintf(file2, "%d", 0);
        fclose(file);
        fclose(file2);

    //    return create_configs("mohsen", "mohsenghasemi8156@gmail.com");
    } else {
        perror("neogit repository has already initialized");
    }
    return 0;
}

bool is_global_usermail = false;
//int tmp_usermail = 0;//baraye inke bedanim dafe chandom ast ke user va email vared mikonim;

//int config(char *username, char *email, int argc, char *argv[]);
int create_configs(char *username, char *email) {
    FILE *file = fopen(".neogit/config", "w");
    if (file == NULL) return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    fprintf(file, "branch: %s", "master");

    fclose(file);
}
int config_username(char *username, int argc, char *argv[]) {
    char directory[1000]; // Change from pointer to array
    char* filename;
    char filepath[1100]; // Increase the buffer size
    if (strcmp(argv[2], "-global") == 0) {
        is_global_usermail = true;
        strcpy(directory, "/home/seyyed"); // Use strcpy to assign value
        filename = "global_config.txt";
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }
    else {
        getcwd(directory, sizeof(directory)); // Use getcwd with correct arguments
        filename = "local_config.txt";
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }

    // Create the file
    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error creating file config");
        return 1;
    }
    // Write data to the file (if needed)
    fprintf(file, "username: %s\n", username);

    // Close the file
    fclose(file);

    printf("File successfully created at: %s\n", filepath);
    return 0;
}

int config_email(char *email, int argc, char *argv[]) {
    char directory[1000]; // Change from pointer to array
    char* filename;
    char filepath[1100]; // Increase the buffer size
    if (strcmp(argv[2], "-global") == 0) {
        is_global_usermail = true;
        strcpy(directory, "/home/seyyed"); // Use strcpy to assign value
        filename = "global_config.txt";
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }
    else {
        getcwd(directory, sizeof(directory)); // Use getcwd with correct arguments
        filename = "local_config.txt";
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }

    // Create the file
    FILE* file = fopen(filepath, "a");
    if (file == NULL) {
        perror("Error creating file config");
        return 1;
    }
    // Write data to the file (if needed)
    fprintf(file, "email: %s\n", email);

    // Close the file
    fclose(file);

    printf("File created successfully at: %s\n", filepath);
    return 0;
}

int create_alias (int argc, char *argv[]) {
    if(strcmp(argv[2], "-global") == 0) {
        FILE *file = fopen("/home/seyyed/global_alias.txt", "a+");
        FILE *file2 = fopen("/home/seyyed/global_commands.txt", "a+");
        if (file == NULL || file2 == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        char alias[50];
        sscanf(argv[3], "alias.%s", alias);
        fprintf(file, "%s\n", alias);
        fprintf(file2, "%s\n", argv[4]);
        fclose(file);
        fclose(file2);
    } else {
        FILE *file = fopen(".neogit/local_alias.txt", "a+");
        FILE *file2 = fopen(".neogit/local_commands.txt", "a+");
        if (file == NULL || file2 == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        char alias[50];
        sscanf(argv[2], "alias.%s", alias);
        printf("%s\n", alias);
        fprintf(file, "%s\n", alias);
        fprintf(file2, "%s\n", argv[3]);
        fclose(file);
        fclose(file2);
    }
    return 0;
}

int run_alias (int argc, char *argv[]) {
    FILE *file = fopen("/home/seyyed/global_alias.txt", "r");
    FILE *file2 = fopen("/home/seyyed/global_commands.txt", "r");
    if (file == NULL || file2 == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    char line[200];
    char line2[200];
    while(fgets(line, sizeof(line), file) != 0){
        fgets(line2, sizeof(line2), file2);
        line[strcspn(line, "\n")] = 0; // Remove newline character
        line2[strcspn(line2, "\n")] = 0;
        if(strcmp(argv[1], line) == 0){
            printf("%s\n", line2);
            system(line2);
            return 0;
        }
    }
    file = fopen(".neogit/local_alias.txt", "r");
    file2 = fopen(".neogit/local_commands.txt", "r");
    if (file == NULL || file2 == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    while(fgets(line, sizeof(line), file) != 0){
        fgets(line2, sizeof(line2), file2);
        line[strcspn(line, "\n")] = 0; // Remove newline character
        line2[strcspn(line2, "\n")] = 0;
        if(strcmp(argv[1], line) == 0){
            printf("%s\n", line2);
            system(line2);
            return 0;
        }
    }
    fclose(file);
    fclose(file2);
    printf("There is no such command\n");
    return 1;
}

int check_existance_repo() {
    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        return 1;
    }

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
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                return 0;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);
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
    //for -undo command
    FILE *file = fopen("/home/seyyed/project/last_add.txt", "w");
    system("cd /home/seyyed/project/");
    system("cat /dev/null > last_add.txt");
    fprintf(file, "%s", filepath);
    return 0;
}

int run_add(int argc, char *argv[], int arg_to_be_add) {
    if (argc < 3) {
        perror("Please specify a file\n");
        return 1;
    }
    
    int result = check_existance_repo();
    if (result != 0) {
        printf("Please create a repo first\n");
        return 1;
    }
    
    return add_to_staging(argv[arg_to_be_add]);
}

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
    printf("The specified file: '%s' doesnt exist.\n", argv[arg_to_be_rm]);
    return 1;
}

int create_commit(int argc, char *argv[]) {
    FILE *file = fopen(".neogit/commit/commit_names.txt", "r+");
    DIR *dir = opendir(".neogit/staging");
    struct dirent *entry;
    int num_of_files_exists_in_staging = 0;
    if(file == NULL) {
        printf("Error opening commit_names.txt");
        return 1;
    }
    char file_name[50];
    int n;
    fscanf(file, "%d", &n);
    n++;
    fprintf(file, "%d", n);
    fclose(file);
    sprintf(file_name, "%d", n);
    char address[200] = ".neogit/commit/";
    strcat(address, file_name);
    printf("%s\n", address);
    if(mkdir(address, 0755) != 0) {
        printf("Errot creating folder");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        num_of_files_exists_in_staging++;
        char command[200] = "mv ";
        strcat(command, ".neogit/staging/");
        strcat(command, entry->d_name);
        strcat(command, " ");
        strcat(command, ".neogit/commit/");
        strcat(command, file_name);
        printf("%s\n", command);
        system(command);
    }
    closedir(dir);
    strcat(file_name, ".txt");
    printf("%s\n", file_name);
    char address2[200] = ".neogit/commit/";
    strcat(address2, file_name);
    FILE *commit_info = fopen(address2, "w");
    char username[50];
    file = fopen("/home/seyyed/global_config.txt", "r");
    if (file == NULL) {
        file = fopen("local_config.txt", "r");
        fscanf(file, "username: %s", username);
        fclose(file);
    } else {
        fscanf(file, "username: %s", username);
        fclose(file);
    }
    char date_str[100] = "2022-01-01-00:00:00";
    struct tm date;
    parse_date(date_str, &date);
    if (!parse_date(date_str, &date)) {
        printf("Error parsing date\n");
        return 1;
    }
    fprintf(commit_info, "commit date and time: %s\n", date_str);
    fprintf(commit_info, "commit message: %s\n", argv[3]);
    fprintf(commit_info, "user: %s\n", username);
    fprintf(commit_info, "commit id: %d\n", n);
    fprintf(commit_info, "number of committed files: %d\n", num_of_files_exists_in_staging);
    file = fopen("commit_names.txt", "w");
    fprintf(file, "%d", n);
    fclose(file);
    return run_commit(argc, argv, address2);
}

int parse_date(char *date_str, struct tm *date) {
    return sscanf(date_str, "%d-%d-%d-%d:%d:%d", &date->tm_year, &date->tm_mon, &date->tm_mday, &date->tm_hour, &date->tm_min, &date->tm_sec) == 6;
}

int run_commit(int argc, char *argv[], char *commitInfoFileName) {
    FILE *file = fopen(commitInfoFileName, "r");
    if (file == NULL) {
        printf("Error opening commitInfo file\n");
        return 1;
    }
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "number of committed files", 24) == 0) break;
        printf("%s\n", line);
    }
    fclose(file);
    return 0;
}

int main (int argc, char *argv[]){

    char username[100]; // Allocate memory for username and email
    char email[100];

    if(argc < 2) {
        printf("Please enter a valid command\n");
        return 1;
    }
    
    else if(strcmp(argv[1], "init") == 0){
        int result = run_init(argc, argv);
        if (result != 0) {
            printf("Repo initialization failed\n");
            return result;
        }
    }
    
    else if (strcmp(argv[1], "config") == 0) {
        //sharte if nesbat be config.c taghiir yafte
        if (strcmp(argv[2], "user.name") == 0) {
            strcpy(username, argv[3]);
            return config_username(username, argc, argv);
        }
        else if (strcmp(argv[3], "user.name") == 0) {
            strcpy(username, argv[4]);
            return config_username(username, argc, argv);
        }
        if (strcmp(argv[2], "user.email") == 0) {
            strcpy(email, argv[3]);
            return config_email(email, argc, argv);
        }
        else if (strcmp(argv[3], "user.email") == 0) {
            strcpy(email, argv[4]);
            return config_email(email, argc, argv);
        }
        if (strncmp(argv[2], "alias", 5) == 0) {
            return create_alias(argc, argv);
        }
        else if (strncmp(argv[3], "alias", 5) == 0) {
            return create_alias(argc, argv);
        }
    }

    else if (strcmp(argv[1], "reset") == 0) {
        if (strcmp(argv[2], "-undo") == 0){
            FILE *file = fopen("/home/seyyed/project/last_add.txt", "r");
            char last_path[231];
            fscanf(file, "%s", last_path);
            char command[200] = "mv .neogit/staging/";
            strcat(command, last_path);
            strcat(command, " ");
            strcat(command, "/home/seyyed/Deleted_because_reset/");
            system(command);
        }
        else if (strcmp(argv[2], "-f") == 0) {
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

    else if (strcmp(argv[1], "commit") == 0) {
        if(argc > 4) {
            printf("Please put your message in double coat\n");
        }
        else if (strlen(argv[3])> 72) {
           printf("The commit message is too long\n"); 
        }
        else if (argc == 3) {
            printf("Please enter a messsage for commit\n");
        }
        else {
            return create_commit (argc, argv);
        }
        
    }

    return 0;
}