#ifndef _ENTER_INCLUDE
#define _ENTER_INCLUDE


#include "Entity.h"
#include "EnterArrow.h"
#include "TileMap.h"
#include "Camera.h"

enum class EnterType {
	DOOR,
	TUNNEL
};

class Enter : public Entity
{
public:
	Enter(EnterType t);
	~Enter();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void render() override;
	void setRoomInt(int r) override;
	void setPosition(const glm::vec2& pos) override;
	void activateArrow() { arrow->activate(); }
	void setConnectedTo(Enter* e) { connectedTo = e; }

	Enter* getConnectedTo() { return connectedTo; }
	EnterType getEnterType() const { return eType; }
	
private:
	EnterType eType;
	Enter* connectedTo = nullptr; // Puerta a la que se conecta esta puerta

protected:
	EnterArrow* arrow = nullptr;
};


#endif // _ENTER_INCLUDE
