#ifndef _GUN_INCLUDE
#define _GUN_INCLUDE

#include "Entity.h"
#include "Camera.h"

class Gun : public Entity
{
public:
	Gun();
	~Gun();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void collect();
	bool isCollected() const { return collecting; }

private:
	bool collecting = false;
	float collectTimer;
};


#endif // _GUN_INCLUDE
