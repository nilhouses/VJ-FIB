#ifndef _WEIGHT_INCLUDE
#define _WEIGHT_INCLUDE


#include "Entity.h"
#include "TileMap.h"
#include "Camera.h"

// El peso contiene
// Sprite, posición, usa tileMap para detectar colisiones, etc.
class Weight : public Entity
{
public:
	Weight();
	~Weight();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void setTileMap(TileMap* tileMap);
	bool incrRight(int units);
	bool incrLeft(int units);


private:
	TileMap* map;						// Mapa de tiles para detectar colisiones
};


#endif // _WEIGHT_INCLUDE


