#pragma once

#define NUM_AUDIOS 3
#ifndef SEMAUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "include/miniaudio.h"
typedef struct {
    ma_decoder decoder;
    ma_device device;
} AudioPlayer;

AudioPlayer audioPlayers[NUM_AUDIOS];

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

void initAudio(AudioPlayer* player, const char* filepath) {
    ma_result result;

    result = ma_decoder_init_file(filepath, NULL, &player->decoder);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize decoder for %s.\n", filepath);
        return;
    }

    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = player->decoder.outputFormat;
    deviceConfig.playback.channels = player->decoder.outputChannels;
    deviceConfig.sampleRate        = player->decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &player->decoder;

    result = ma_device_init(NULL, &deviceConfig, &player->device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize playback device for %s.\n", filepath);
        ma_decoder_uninit(&player->decoder);
        return;
    }
}

void startAudio(AudioPlayer* player) {
    ma_device_start(&player->device);
}

void stopAudio(AudioPlayer* player) {
    ma_device_stop(&player->device);
}

void initializeAudios() {
    // Inicialize os diferentes áudios
    initAudio(&audioPlayers[0], "audios/menu.mp3");
    initAudio(&audioPlayers[1], "audios/gameover.mp3");
    initAudio(&audioPlayers[2], "audios/game.mp3");
}

void cleanupAudios() {
    for (int i = 0; i < NUM_AUDIOS; i++) {
        ma_device_uninit(&audioPlayers[i].device);
        ma_decoder_uninit(&audioPlayers[i].decoder);
    }
}
#else
typedef int AudioPlayer;
AudioPlayer audioPlayers[NUM_AUDIOS];
void initAudio(AudioPlayer* player, const char* filepath){}
void startAudio(AudioPlayer* player){}
void stopAudio(AudioPlayer* player){}
void initializeAudios(){}
void cleanupAudios(){}
#endif