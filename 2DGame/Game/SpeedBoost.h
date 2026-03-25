#ifndef _SPEEDBOOST_INCLUDE
#define _SPEEDBOOST_INCLUDE


#include "Entity.h"
#include "Camera.h"

class SpeedBoost : public Entity
{
public:
	SpeedBoost();
	~SpeedBoost();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
	void update(int deltaTime) override;
	void collect();
	bool isCollected() const { return collecting; }
	float getMultiplier() const { return multiplier; }
	float getDuration() const { return duration; }
	static float getMaxTime() { return 12500.f; }

private:
	bool collecting = false;
	float collectTimer;
	static constexpr float multiplier = 2.f;
	static constexpr float duration = 12500.f; 
};


#endif // _SPEEDBOOST_INCLUDE

