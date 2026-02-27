#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE


#include "Entity.h"
#include "TileMap.h"
#include "Camera.h"


// La llave contiene
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Key : public Entity
{
public:
	Key();
	~Key();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	void Key::setTileMap(TileMap* tileMap);

private:
	bool isCollected;				    // Indica si la llave se ha cogido
	TileMap* map;						// Mapa de tiles para detectar colisiones
};


#endif // _KEY_INCLUDE


