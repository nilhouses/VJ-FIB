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
	void update();      // Limpia sonidos que ya han terminado

    // Para efectos cortos (saltos, llaves, explosiones)
    void playSound(const std::string& sound, float volume);

    // Para música de fondo (en bucle)
    void playMusic(bool loop = true);
    void stopMusic();

    // Ajusta el volumen de TODO el motor (0.0 a 1.0)
    void setMasterVolume(float volume);

    // Ajusta el volumen de la música actual
    void setMusicVolume(float volume);


private:
    SoundManager() : initialized(false) {} // Constructor privado
    ma_engine engine;
    ma_sound bgm; // Objeto específico para la música actual
    bool initialized;
    bool musicPlaying = false;

    float musicVolume = 0.f;
    std::list<ma_sound*> activeSounds; // Lista de sonidos en reproducción
};

#endif