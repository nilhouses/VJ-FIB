#define MINIAUDIO_IMPLEMENTATION
#include "SoundManager.h"
#include <iostream>

// Sonidos predefinidos
std::unordered_map<std::string, std::string> sounds = {
    { "horse", "sounds/caballoMVP.mp3"},
    { "background", "sounds/bonprofit_remix.mp3" }
};


void SoundManager::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Error: No se pudo iniciar el motor de audio." << std::endl;
        return;
    }
    initialized = true;
}

void SoundManager::playSound(const std::string& sound) {
    if (!initialized) return;
    // Reproduce un sonido y se olvida (el motor gestiona la memoria solo)
	auto it = sounds.find(sound);
    ma_engine_play_sound(&engine, it->second.c_str(), NULL);
}

void SoundManager::playMusic(bool loop) {
    if (!initialized) return;

    if (musicPlaying) stopMusic();

    // Cargamos la música como "Stream" (para que no ocupe mucha RAM)
	auto it = sounds.find("background");
    ma_result result = ma_sound_init_from_file(&engine, it->second.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &bgm);
    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&bgm, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start(&bgm);
        musicPlaying = true;
    }
}

void SoundManager::stopMusic() {
    if (musicPlaying) {
        ma_sound_stop(&bgm);
        ma_sound_uninit(&bgm);
        musicPlaying = false;
    }
}

void SoundManager::shutdown() {
    if (initialized) {
        if (musicPlaying) stopMusic();
        ma_engine_uninit(&engine);
    }
}