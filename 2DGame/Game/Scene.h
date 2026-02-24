#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

// Esta clase contiene todas las entidades de un nivel y es responsable de actualizarlas y renderizarlas.

// Scene es ahora una clase abstracta (interface).
// Cualquier pantalla nueva (Menú, Juego, Créditos) hereda de Scene.
class Scene
{

public:
	virtual ~Scene() {} // El destructor es virtual porque vamos a usar punteros a Scene para manejar las diferentes pantallas del juego (menú, juego, créditos...)
	
	// Estas funciones son virtuales puras, lo que obliga a que cualquier clase que herede de Scene tenga que implementarlas. De esta forma, cada pantalla del juego (menú, juego, créditos...) implementa su propia versión de estas funciones.
	virtual void init() = 0; // Muestra el mapa y el jugador en la posición inicial
	virtual void update(int deltaTime) = 0; // Actualiza el mapa y el jugador (en este caso solo al jugador, mapa estático)	
	virtual void render() = 0; // Pinta el mapa y el jugador
};


#endif // _SCENE_INCLUDE

