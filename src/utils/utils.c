#include "utils.h"
#include "constants.h"
#include "mainLibraries.h"

//useful libraries for password hashing
#include <openssl/evp.h>
#include <openssl/sha.h>

int password_strength(const char *password) {
    int length = strlen(password);
    int has_lower = 0, has_upper = 0, has_digit = 0, has_special = 0;
    int score = 0;

    for (int i = 0; password[i]; ++i) {
        if (islower(password[i])) has_lower = 1;
        else if (isupper(password[i])) has_upper = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else if (ispunct(password[i]) || isspace(password[i])) has_special = 1;
    }

    if (length >= 8) score++;
    if (length >= 12) score++;
    if (has_lower && has_upper) score++;
    if (has_digit) score++;
    if (has_special) score++;

    // Scoring logic
    if (score <= 2) return 0;           // Weak
    else if (score == 3) return 1;      // Medium
    else if (score == 4) return 2;      // Strong
    else return 3;                      // Very Strong
}

int generate_password_hash(const char *password, char *hashed_password) {
    // Hash the password using SHA-256
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)password, strlen(password), hash);

    // Convert hash to hex string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashed_password + (i * 2), "%02x", hash[i]);
    }
    hashed_password[SHA256_DIGEST_LENGTH * 2] = '\0';
    return 0; // Success
}

int verify_password(const char *password, const char *hashed_password) {
    char computed_hash[SHA256_DIGEST_LENGTH * 2 + 1];
    generate_password_hash(password, computed_hash);
    return strcmp(computed_hash, hashed_password) == 0;
}

int check_if_common(const char *password) {
    int success = 0;
    FILE *file = fopen(COMMON_PASSWORDS_FILE, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening common passwords file.\n");
        return -1; // Error
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        if (strcmp(password, line) == 0) {
            success = 1; // Password is common
            break;
        }
    }

    fclose(file);
    return success;
}

//function for initializing portaudio
int initialize_portaudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return -1; // Error
    }
    return 0; // Success
}

//function for closing portaudio
int close_portaudio() {
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return -1; // Error
    }
    return 0; // Success
}

//create a directory if it doesn't exist
int create_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0777) == -1) {
            fprintf(stderr, "Error creating directory: %s\n", path);
            return -1; // Error
        }
    }
    return 0; // Success
}

int files_in_directory_list(const char *path, char*** files_list) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    if (dp == NULL) {
        fprintf(stderr, "Error opening directory: %s\n", path);
        return -1; // Error
    }

    int count = 0;
    *files_list = malloc(sizeof(char*) * MAX_FILES_PER_USER);
    if (*files_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        closedir(dp);
        return -1; // Error
    }

    while ((entry = readdir(dp)) != NULL) {
        char fullpath[FILE_PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode)) {
            if (count >= MAX_FILES_PER_USER) {
                fprintf(stderr, "Maximum number of files exceeded\n");
                break; // Limit reached
            }
            size_t len = strlen(entry->d_name) + 1;
            (*files_list)[count] = malloc(len);
            if ((*files_list)[count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                closedir(dp);
                return -1;
            }
            strcpy((*files_list)[count], entry->d_name);
            count++;
        }
    }

    closedir(dp);
    return count; // Success
}