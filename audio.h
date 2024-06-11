#pragma once

enum Audios{
	A_MENU,
	A_GAMEOVER,
	A_JOGO,
	A_CHOCK
};

#ifndef SEMAUDIO
#define NUM_AUDIOS 4
#define MINIAUDIO_IMPLEMENTATION
#include "include/miniaudio.h"
typedef struct {
    ma_decoder decoder;
    ma_device device;
    bool tocando;
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

void initAudio(Audios audio, const char* filepath, ma_device_type type) {
    AudioPlayer *player = audioPlayers + audio;
    ma_result result;

    result = ma_decoder_init_file(filepath, NULL, &player->decoder);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize decoder for %s.\n", filepath);
        return;
    }

    ma_device_config deviceConfig;
    deviceConfig = ma_device_config_init(type);
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

void startAudio(Audios audio, bool loop) {
    if(!audioPlayers[audio].tocando){
        ma_device_start(&audioPlayers[audio].device);
        audioPlayers[audio].tocando = loop;
    }
}

void stopAudio(Audios audio) {
    if(audioPlayers[audio].tocando){
        ma_device_stop(&audioPlayers[audio].device);
        audioPlayers[audio].tocando = false;
    }
}

void initializeAudios() {
    // Inicialize os diferentes áudios
    initAudio(A_MENU, "audios/menu.mp3", ma_device_type_loopback);
    initAudio(A_GAMEOVER, "audios/gameover.mp3", ma_device_type_loopback);
    initAudio(A_JOGO, "audios/game.mp3", ma_device_type_loopback);
    initAudio(A_CHOCK, "audios/shock.mp3", ma_device_type_playback);
}

void cleanupAudios() {
    for (int i = 0; i < NUM_AUDIOS; i++) {
        ma_device_uninit(&audioPlayers[i].device);
        ma_decoder_uninit(&audioPlayers[i].decoder);
    }
}
#else
void startAudio(Audios audio, bool loop){}
void stopAudio(Audios audio){}
void initializeAudios(){}
void cleanupAudios(){}
#endif