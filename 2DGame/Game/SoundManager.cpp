#define MINIAUDIO_IMPLEMENTATION
#include "SoundManager.h"
#include <iostream>

// Sonidos predefinidos
unordered_map<string, string> sounds = {
    { "horse", "sounds/caballoMVP.mp3"},
    { "background", "sounds/intro.mp3"}//    bonprofit_remix.mp3" }
};


void SoundManager::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        cout << "Error: No se pudo iniciar el motor de audio." << endl;
        return;
    }
    initialized = true;
}

void SoundManager::playSound(const string& sound, float volume) {
    if (!initialized) return;

    auto it = sounds.find(sound);
    if (it == sounds.end()) {
        cout << "Error: Sonido '" << sound << "' no encontrado en el mapa." << endl;
        return;
    }

    // 1. Creamos un nuevo objeto de sonido en el heap
    ma_sound* sfx = new ma_sound();

    // 2. Lo inicializamos (usamos la ruta de tu mapa: it->second)
    ma_result result = ma_sound_init_from_file(&engine, it->second.c_str(), 0, NULL, NULL, sfx);

    if (result == MA_SUCCESS) {
        // 3. Aplicamos el volumen y lo lanzamos
        ma_sound_set_volume(sfx, volume);
        ma_sound_start(sfx);

        // 4. Lo guardamos en la lista para que no se destruya al salir de la función
        activeSounds.push_back(sfx);
    }
    else {
        delete sfx; // Limpieza si falla la carga
    }
}

void SoundManager::playMusic(bool loop) {
    if (!initialized) return;

    if (musicPlaying) stopMusic();

    // Cargamos la música como "Stream" (para que no ocupe mucha RAM)
	auto it = sounds.find("background");
    ma_result result = ma_sound_init_from_file(&engine, it->second.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &bgm);
    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&bgm, loop ? MA_TRUE : MA_FALSE);
		ma_sound_set_volume(&bgm, musicVolume);
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

void SoundManager::setMasterVolume(float volume) {
    if (!initialized) return;
    ma_engine_set_volume(&engine, volume);
}

void SoundManager::setMusicVolume(float volume) {
    if (!initialized) return;
    musicVolume = volume;
    if (musicPlaying) {
        ma_sound_set_volume(&bgm, volume);
    }
}


void SoundManager::update() {
    if (!initialized) return;

    // Recorremos la lista y eliminamos los que ya han terminado de sonar
    auto it = activeSounds.begin();
    while (it != activeSounds.end()) {
        if (ma_sound_at_end(*it)) {
            ma_sound_uninit(*it); // Libera recursos de miniaudio
            delete* it;           // Libera memoria de C++
            it = activeSounds.erase(it); // Lo quita de la lista
        }
        else {
            ++it;
        }
    }
}