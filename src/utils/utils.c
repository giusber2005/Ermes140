#include "utils.h"
#include "constants.h"
#include "mainLibraries.h"

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

