#ifndef _PLATFORM_INCLUDE
#define _PLATFORM_INCLUDE
#include "Entity.h"
#include "Camera.h"

class Platform : public Entity
{
public:
    Platform();
    ~Platform();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int rangePixels, int axis, int direction, int sr); // axis: 0=horizontal, 1=vertical, dirección: 1 o -1
    void update(int deltaTime) override;
    glm::vec2 getDeltaMovement() const { return deltaMovement; }
    
    virtual void setPosition(const glm::vec2& pos) override;
    bool isHorizontal() { return axis == 0; }

private:
    glm::vec2 origin = glm::vec2(0.f,0.f);          // Posición inicial
	int rangePixels = 0;                            // Rango de movimiento en píxeles
    int axis = 0;                                   // 0 = horitzontal, 1 = vertical
    int direction = 1;                              // 1 o -1
    glm::vec2 deltaMovement = glm::vec2(0.f,0.f);
    float distanceCounter = 0.f;
};

#endif // _PLATFORM_INCLUDE