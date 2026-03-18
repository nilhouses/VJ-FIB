#include "Platform.h"

#define PLATFORM_SPEED 1.5f
#define PAUSE_TIME 0.5f


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

    float moveTime = (float)rangePixels / 80.0f; // 80px/s
    float totalHalfCycle = moveTime + PAUSE_TIME;
    float angularFrequency = 3.14159f / totalHalfCycle;

    distanceCounter += (deltaTime / 1000.0f) * angularFrequency;

    float amp = 1.0f / cos(angularFrequency * PAUSE_TIME * 0.5f);

    float wave = cos(distanceCounter) * amp;

    if (wave > 1.0f) wave = 1.0f;
    else if (wave < -1.0f) wave = -1.0f;

    float multiplier = (1.0f - wave) * 0.5f;
    float offset = (float)rangePixels * multiplier;

    if (axis == 0) pos.x = (int)(origin.x + (offset * direction));
    else pos.y = (int)(origin.y + (offset * direction));

    deltaMovement = glm::vec2(pos.x - prevPos.x, pos.y - prevPos.y);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}