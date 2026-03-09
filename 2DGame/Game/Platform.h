#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE
#include "Entity.h"
#include "Camera.h"

class Platform : public Entity
{
public:
    Platform();
    ~Platform();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int rangePixels, int axis, int direction); // axis: 0=horizontal, 1=vertical, dirección: 1 o -1
    void update(int deltaTime) override;
    glm::vec2 getDeltaMovement() const { return deltaMovement; }
    
    virtual void setPosition(const glm::vec2& pos) override;

private:
    glm::vec2 origin;       // Posición inicial
	int rangePixels;        // Rango de movimiento en píxeles
    int axis;               // 0 = horitzontal, 1 = vertical
    int direction;          // 1 o -1
    glm::vec2 deltaMovement;
    float distanceCounter = 0.f;
};

#endif // _PLATFORM_INCLUDE