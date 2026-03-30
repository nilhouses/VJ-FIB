#ifndef _LIFE_INCLUDE
#define _LIFE_INCLUDE


#include "Entity.h"
#include "Camera.h"

class Life : public Entity
{
public:
	Life();
	~Life();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void collect();
	bool isCollected() const { return collecting; }
	
private:
	bool collecting = false;
	float collectTimer = 0.f;
};


#endif // _LIFE_INCLUDE

