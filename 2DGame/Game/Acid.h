#ifndef _ACID_INCLUDE
#define _ACID_INCLUDE

#include "Entity.h"

class Acid : public Entity
{
public:
	Acid();
	~Acid();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	void setPosition(const glm::vec2& pos) override;

private:
	glm::vec2 initPos = glm::vec2(0.f, 0.f);
	float timer = 0;
};


#endif // _ACID_INCLUDE