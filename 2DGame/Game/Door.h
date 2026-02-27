#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Entity.h"
#include "TileMap.h"
#include "Camera.h"


class Door : public Entity
{
public:
	Door();
	~Door();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	void setToVisited();
	bool getVisited() { return visited; }

private:
	bool visited;
};


#endif // _KEY_INCLUDE

