#ifndef _ENTERARROW_INCLUDE
#define _ENTERARROW_INCLUDE


#include "Entity.h"
#include "Camera.h"


class EnterArrow: public Entity
{
public:
	EnterArrow();
	~EnterArrow();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool pointsDown = false);
	void update(int deltaTime) override;

private:
	bool active = false;
};


#endif // _ENTERARROW_INCLUDE


