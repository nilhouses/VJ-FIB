#define MINIAUDIO_IMPLEMENTATION
#include "SoundManager.h"
#include <iostream>
using namespace std;

// Sonidos predefinidos
std::unordered_map<std::string, std::string> sounds = {
    { "menu", "sounds/ost/intro.mp3"},
    { "levels1&2", "sounds/ost/desierto.mp3"},
    { "levels3&4", "sounds/ost/cueva.mp3"},
    { "level5", "sounds/ost/gusano.mp3"},
    { "death", "sounds/death.mp3"},
    { "gameOver", "sounds/gameOver.mp3"},
    { "life", "sounds/life.mp3"},
    { "key", "sounds/key.mp3"},
    { "explosion", "sounds/explosion.mp3"},
    { "kachow", "sounds/kachow.mp3"},
    { "gun", "sounds/gun.mp3"},
    { "shot", "sounds/shot.mp3"},
    { "doorOpen", "sounds/doorOpen.mp3"},
    { "clothDoorOpen", "sounds/clothDoorOpen.mp3"},
    { "cowboyDoorOpen", "sounds/cowboyDoorOpen.mp3"},
    { "stepIn", "sounds/caveDoor.mp3"},
    { "tunnelSteps", "sounds/tunnelSteps.mp3"},
    { "pipe_in", "sounds/pipe_in.mp3"},
    { "pipe_out", "sounds/pipe_out.mp3"},
    { "doorLocked", "sounds/doorLocked.mp3"},
    { "invalidAction", "sounds/invalidAction.mp3"},
    { "openLockedDoor", "sounds/openLockedDoor.mp3"},
    { "openChainedDoor", "sounds/chainDoor.mp3"},
    { "openSpaceShipDoor", "sounds/spaceShipDoor.mp3"},
    { "chainDoorLocked", "sounds/chainLockedDoor.mp3"}
};


void SoundManager::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cout << "Error: No se pudo iniciar el motor de audio." << std::endl;
        return;
    }
    initialized = true;
}

void SoundManager::playSound(const std::string& sound, float volume) {
    if (!initialized) return;

    auto it = sounds.find(sound);
    if (it == sounds.end()) {
        std::cout << "Error: Sonido '" << sound << "' no encontrado en el mapa." << std::endl;
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

void SoundManager::playMusic(const std::string& musicKey, bool loop) {
    if (!initialized) return;

    if (musicPlaying) {
        if (currentMusicKey == musicKey) return; // Ja sona, no reiniciar
        stopMusic();
    }
    // Cargamos la música como "Stream" (para que no ocupe mucha RAM)
    auto it = sounds.find(musicKey);
    if (it == sounds.end()) {
        std::cout << "Error: Música '" << musicKey << "' no trobada." << std::endl;
        return;
    }
    ma_result result = ma_sound_init_from_file(&engine, it->second.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &bgm);
    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&bgm, loop ? MA_TRUE : MA_FALSE);
        ma_sound_set_volume(&bgm, musicVolume);
        ma_sound_start(&bgm);
        musicPlaying = true;
        currentMusicKey = musicKey;
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


void SoundManager::increaseMusicVolume(float delta) {
    setMusicVolume(min(musicVolume + delta, 1.0f));
}

void SoundManager::decreaseMusicVolume(float delta) {
    setMusicVolume(max(musicVolume - delta, 0.0f));
}