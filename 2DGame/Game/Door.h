#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE


#include "Enter.h"
#include "TileMap.h"
#include "Camera.h"


class Door : public Enter
{
public:
	Door();
	~Door();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);

	void setToVisited();
	bool getVisited() { return visited; }

	void setIsFinalDoor(bool isFinal);
	bool getIsFinalDoor() { return isFinalDoor; }

private:
	bool visited;
	bool isFinalDoor;
};


#endif // _DOOR_INCLUDE

