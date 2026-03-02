#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include "miniaudio.h"
#include <string>
#include <unordered_map>
#include <map>

class SoundManager {
public:
    // Acceso global al Manager
    static SoundManager& instance() {
        static SoundManager inst;
        return inst;
    }

    void init();        // Inicializa el motor
    void shutdown();    // Apaga todo al cerrar el juego

    // Para efectos cortos (saltos, llaves, explosiones)
    void playSound(const std::string& sound);

    // Para música de fondo (en bucle)
    void playMusic(bool loop = true);
    void stopMusic();

private:
    SoundManager() : initialized(false) {} // Constructor privado
    ma_engine engine;
    ma_sound bgm; // Objeto específico para la música actual
    bool initialized;
    bool musicPlaying = false;
};

#endif