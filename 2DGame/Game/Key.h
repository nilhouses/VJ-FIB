#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE


#include "Entity.h"
#include "Camera.h"


// La llave contiene
// Sprite, posición
class Key : public Entity
{
public:
	Key();
	~Key();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
};


#endif // _KEY_INCLUDE


