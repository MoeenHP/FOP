#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>

bool is_global_usermail = false;
int tmp_usermail = 0;//baraye inke bedanim dafe chandom ast ke user va email vared mikonim;

//int config(char *username, char *email, int argc, char *argv[]);

int config_username(char *username, int argc, char *argv[]) {
    char directory[1000]; // Change from pointer to array
    char* filename;
    char filepath[1100]; // Increase the buffer size
    if (strcmp(argv[2], "-global") == 0) {
        is_global_usermail = true;
        strcpy(directory, "C:\\newrealproject"); // Use strcpy to assign value
        filename = "global_config.txt";
        snprintf(filepath, sizeof(filepath), "%s\\%s", directory, filename);
    }
    else {
        getcwd(directory, sizeof(directory)); // Use getcwd with correct arguments
        filename = "local_config.txt";
        printf("%s\n", directory);
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }

    // Create the file
    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Error creating file config");
        return 1;
    }
    printf("%s\n", directory);
    // Write data to the file (if needed)
    fprintf(file, "username is : %s\n", username);

    // Close the file
    fclose(file);

    printf("File created successfully at: %s\n", filepath);
    return 0;
}

int config_email(char *email, int argc, char *argv[]) {
    char directory[1000]; // Change from pointer to array
    char* filename;
    char filepath[1100]; // Increase the buffer size
    if (strcmp(argv[2], "-global") == 0) {
        is_global_usermail = true;
        strcpy(directory, "C:\\newrealproject"); // Use strcpy to assign value
        filename = "global_config.txt";
        snprintf(filepath, sizeof(filepath), "%s\\%s", directory, filename);
    }
    else {
        getcwd(directory, sizeof(directory)); // Use getcwd with correct arguments
        filename = "local_config.txt";
        printf("%s\n", directory);
        snprintf(filepath, sizeof(filepath), "%s/%s", directory, filename);
    }

    // Create the file
    FILE* file = fopen(filepath, "a");
    if (file == NULL) {
        perror("Error creating file config");
        return 1;
    }
    printf("%s\n", directory);
    // Write data to the file (if needed)
    fprintf(file, "email is: %s\n", email);

    // Close the file
    fclose(file);

    printf("File created successfully at: %s\n", filepath);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 4) { // Check if there are enough arguments
        printf("Insufficient arguments\n");
        return 1;
    }

    char username[100]; // Allocate memory for username and email
    char email[100];

    if (strcmp(argv[1], "config") == 0 && ((strncmp(argv[2], "alias", 5) != 0) || (strncmp(argv[3], "alias", 5) != 0))) {
        if (strcmp(argv[2], "user.name") == 0) {
            strcpy(username, argv[3]);
            config_username(username, argc, argv);
        }
        else if (strcmp(argv[3], "user.name") == 0) {
            strcpy(username, argv[4]);
            config_username(username, argc, argv);
        }
        if (strcmp(argv[2], "user.email") == 0) {
            strcpy(email, argv[3]);
            config_email(email, argc, argv);
        }
        else if (strcmp(argv[3], "user.email") == 0) {
            strcpy(email, argv[4]);
            config_email(email, argc, argv);
        }
    }

    return 0;
}
