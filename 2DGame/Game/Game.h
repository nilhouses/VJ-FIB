#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application

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

	bool getKey(int key) const;

private:
	bool bPlay;					// Guarda si el juego debe seguir ejecutandose o no
	bool keys[GLFW_KEY_LAST+1]; // Guarda por cada tecla si está pulsada o no
	Scene scene;				// Escena del juego (solo hay una por lo que tenemos un nivel)

};


#endif // _GAME_INCLUDE


