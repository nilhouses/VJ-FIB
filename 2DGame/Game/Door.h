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
	void showArrow() { arrow->activate(); }

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int sr);
	void setToVisited();
	bool getVisited() { return visited; }
	bool getIsFinalDoor() { return getConnectedTo() == nullptr; }
	void openingAnim(bool sound);
	bool isCave() { return spriteRow == 4 || spriteRow == 6; } // La 8 no la pongo para que haga OPEN_AND_ENTER
	bool isWorm() { return spriteRow == 9; }
	void lockedDoorSound();
	void openLockedDoorSound();

private:
	bool visited;
	int spriteRow;
};


#endif // _DOOR_INCLUDE

