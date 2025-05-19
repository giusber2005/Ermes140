#ifndef REPRODUCE_H
#define REPRODUCE_H

#include "../../utils/mainLibraries.h"
#include "../../utils/constants.h"
#include "../management/management.h"

// Function to play audio
bool reproduce_audio(sqlite3 *db, const char *audio_name);

#endif

