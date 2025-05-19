#include "reproduce.h"

//function to play audio files
bool reproduce_audio(char *audio_name) {
    SNDFILE *file;
    SF_INFO sfinfo;
    float buffer[FRAMES_PER_BUFFER * 2]; // stereo
    PaStream *stream;

    // Open audio file
    char *audio_path = malloc(strlen(AUDIOS_FOLDER_PATH) + strlen(audio_name) + 1);
    if (audio_path == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    strcpy(audio_path, AUDIOS_FOLDER_PATH);
    strcat(audio_path, audio_name);
    file = sf_open(audio_path, SFM_READ, &sfinfo);
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }
    
    Pa_OpenDefaultStream(&stream, 0, sfinfo.channels, paFloat32, sfinfo.samplerate, FRAMES_PER_BUFFER, NULL, NULL);
    Pa_StartStream(stream);

    sf_count_t readcount;
    while ((readcount = sf_readf_float(file, buffer, FRAMES_PER_BUFFER)) > 0) {
        Pa_WriteStream(stream, buffer, readcount);
    }

    // Clean up
    free(audio_path);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    sf_close(file);

    return true;
}
