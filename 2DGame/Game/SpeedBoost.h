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
	float getTimeActive() const { return timeActive; }

private:
	bool collecting = false;
	float collectTimer;
	float multiplier = 2.f; // Valor por defecto, yo haría que todos los boosts tuviesen el mismo multiplicador y duracion
	float timeActive = 12500.f; // 12.5 segundos 

};


#endif // _SPEEDBOOST_INCLUDE

