#include "Platform.h"

#define PLATFORM_SPEED 1.5f

Platform::Platform() : Entity(Type::PLATFORM) {}

Platform::~Platform()
{
    if (sprite != NULL)
        delete sprite;
}

void Platform::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int rangePixels, int axis, int direction)
{
    Entity::init(tileMapPos, shaderProgram, "images/platform.png", glm::ivec2(64, 16), glm::vec2(1.f, 1.f), c);

    this->rangePixels = rangePixels;
    this->axis = axis;
    this->direction = (direction >= 0) ? 1 : -1;
    this->distanceCounter = 0.0f;

    sprite->setNumberAnimations(1);
    sprite->setAnimationSpeed(0, 1);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->changeAnimation(0);
}

void Platform::setPosition(const glm::vec2& pos) {
    Entity::setPosition(pos);
    this->origin = pos;
}


void Platform::update(int deltaTime)
{
    sprite->update(deltaTime);
    glm::vec2 prevPos = pos;

	// Usamos los p�xeles recorridos para calcular la posici�n, de esta forma el movimiento es independiente de los fps
    distanceCounter += PLATFORM_SPEED * (deltaTime / 16.0f);

    float totalCycle = rangePixels * 2.0f;
    float currentOffset = fmod(distanceCounter, totalCycle);

    if (currentOffset > rangePixels) currentOffset = totalCycle - currentOffset;

    if (axis == 0) pos.x = (int) (origin.x + (currentOffset * direction));
    else           pos.y = (int) (origin.y + (currentOffset * direction));

    deltaMovement = glm::vec2(pos.x - prevPos.x, pos.y - prevPos.y);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}