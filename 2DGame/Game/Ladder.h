#ifndef _LADDER_INCLUDE
#define _LADDER_INCLUDE


#include "Entity.h"

class Ladder : public Entity
{
public:
	Ladder();
	~Ladder();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;

	bool contains(const glm::vec2& playerPos) const;

private:

};


#endif // _LADDER_INCLUDE

