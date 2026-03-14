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
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int sr);
	void setToVisited();
	bool getVisited() { return visited; }

	void setIsFinalDoor(bool isFinal);
	bool getIsFinalDoor() { return isFinalDoor; }

	void openingAnim();

private:
	bool visited;
	bool isFinalDoor;
	int spriteRow;
};


#endif // _DOOR_INCLUDE

