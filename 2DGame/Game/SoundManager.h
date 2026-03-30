#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include "miniaudio.h"
#include <list>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <cmath>

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
	void playSoundForce(const std::string& sound, float volume);

    // Para música de fondo (en bucle)
    void playMusic(const std::string& musicKey, bool loop = true);
    void stopMusic();

    // Ajusta el volumen de todo el motor
    void setMasterVolume(float volume);
    
    // Ajusta el volumen de la música actual
    void setMusicVolume(float volume);
    void setMusicSpeed(float speed);

    void increaseVolume(float delta);
    void decreaseVolume(float delta);

	bool thereIsSoundPlaying() const { return musicPlaying || !activeSounds.empty(); }

private:
    SoundManager() : initialized(false) {} // Constructor privado
    ma_engine engine;
    ma_sound bgm;
    bool initialized = false;
    bool musicPlaying = false;
    std::string currentMusicKey = "";
    float musicVolume = 0.1f;
    float turboTimer = 0.0f;
    std::list<ma_sound*> activeSounds; // Lista de sonidos en reproducción
};

#endif