#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "Camera.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

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
	virtual void init() = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

    // Resize se hará tanto en menús como en partidas para conservar la RA, por lo que se define aquí mismo.
    virtual void resize(int width, int height, int raW, int raH) {
        float scale = std::min(width / (float)raW, height / (float)raH);
        int viewWidth = (int)(raW * scale);
        int viewHeight = (int)(raH * scale);
        int viewX = (width - viewWidth) / 2;
        int viewY = (height - viewHeight) / 2;
        glViewport(viewX, viewY, viewWidth, viewHeight);
        projection = glm::ortho(0.f, float(raW), float(raH), 0.f);
    }
protected:
    glm::mat4 projection;
};


#endif // _SCENE_INCLUDE

