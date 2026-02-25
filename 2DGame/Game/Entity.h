#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"

class Entity
{
public:
	Entity();
	~Entity();

public:
	// Todas las entidades se inicializan con la misma función init. En este punto la posición es fija respecto al mapa de tiles
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spritesheetDir, const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet);
	
	// La función de update es abstracta y cada entidad tiene su propia implementación
	virtual void update(int deltaTime) = 0;
	
	// La función de render es común para todas porque delega la lógica de renderizado al sprite, que es el mismo para todas las entidades. De esta forma, cada entidad solo tiene que implementar la función de update con su propia lógica.
	void render();

	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition() const { return pos; }

	// Función para activar o desactivar la entidad. Si una entidad está desactivada, no se renderiza ni se actualiza, pero sigue existiendo en el juego (por ejemplo, un enemigo que ha sido derrotado pero que sigue existiendo en el mapa).
	void deactivate() { active = false; }
	// Devuelve si está activo o no
	bool isActive() const { return active; }

protected:
	glm::ivec2 tileMapDispl, pos;			// Posición de la entidad en el mapa de tiles
	Texture spritesheet;
	Sprite *sprite;
	bool active;							// Indica si la entidad está activa (visible y actualizable) o no
};

#endif // _ENTITY_INCLUDE