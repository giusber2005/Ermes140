#ifndef UTILS_H
#define UTILS_H

int generate_password_hash(const char *password, char *hashed_password);
int verify_password(const char *password, const char *hashed_password);
int password_strength(const char *password);
int check_if_common(const char *password);

int initialize_portaudio();
int close_portaudio();

int create_directory(const char *path);
int files_in_directory_list(const char *path, char*** file_list);

#endif