#ifndef _TRANSITION_INCLUDE
#define _TRANSITION_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

class Transition {
public:
    virtual ~Transition() {}
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;
    virtual bool isFinished() const = 0;
};

#endif