#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Transition.h"


#define LEVEL_WIDTH 640
#define LEVEL_HEIGHT 384
#define HUD_WIDTH 640
#define HUD_HEIGHT 96
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT (LEVEL_HEIGHT + HUD_HEIGHT) // 480


// Game is a singleton (a class with a single instance) that represents our whole application

enum GameState {
	MAIN_MENU,
	PLAYING,
	INSTRUCTIONS,
	CREDITS,
	LOADING
};

// Hay que entender la clase Game como el controlador global del juego. Es el encargado de cambiar entre escenas, actualizar el estado del juego...
class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
		return G;
	}
	
	void init();					// Inicializa el juego
	bool update(int deltaTime);		// Actualiza el estado del juego. Devuelve false si el juego se tiene que cerrar
	void render();					// Dibuja el nuevo frame en función del estado del juego
	
	// Estas funciones guardan el estado de cada tecla y botón del ratón, en este caso solo actualiza la variable keys.
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void resize(int width, int height);
	void startTransition(FadeType type, GameState newState, int levelNumber); // Inicia una transición a newState, cargando el nivel indicado por levelNumber si el nuevo estado es PLAYING

	bool getKey(int key) const;

	// Cambia el estado del juego a newState. Si el nuevo estado es PLAYING, se carga el nivel indicado por levelNumber (por defecto el 1)
	void changeState(GameState newState, int levelNumber = 1);
private:
	bool bPlay;					 // Guarda si el juego debe seguir ejecutandose o no
	bool keys[GLFW_KEY_LAST+1];  // Guarda por cada tecla si está pulsada o no
	Scene* currentScene;         // Puntero a la pantalla a la que estamos
	int currentLevel;            // Nivel actual
	int numLives = 3;            // Número de vidas del jugador en la partida actual, se mantiene al cambiar de nivel  

	// Gestión de la transición entre escenas
	Transition* currentTransition;
	GameState pendingState;         // El estado al que se cambiará una vez termine la transición
	int pendingLevel;				// El nivel que se cargará al cambiar a PLAYING después de la transición
	bool isTransitioning;			// Indica si estamos en medio de una transición
};


#endif // _GAME_INCLUDE


