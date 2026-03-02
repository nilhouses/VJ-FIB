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
	void setRoomTo(int room) { roomTo = room; }
	int getRoomTo() { return roomTo; }
	void setDoorTargetPosition(const glm::vec2& pos) { doorTargetPosition = pos; }
	glm::vec2 getDoorTargetPosition() { return doorTargetPosition; }

private:
	bool visited;
	int roomTo;						// Número de habitación a la que lleva la puerta
	glm::vec2 doorTargetPosition;	// Posición a la que se quiere posicionar el jugador al entrar por esta puerta (en tiles)
};


#endif // _KEY_INCLUDE

