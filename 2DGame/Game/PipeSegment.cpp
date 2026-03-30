#include "PipeSegment.h"


void PipeSegment::init(Sprite* s, const glm::vec2& idleTexCoords, const glm::vec2& busyTexCoords, const glm::vec2& tilePos)
{
    sprite = s;
    this->tilePos = tilePos;

    sprite->setNumberAnimations(PIPE_NUM_ANIMS);
    sprite->setAnimationSpeed(PIPE_IDLE, 1);
    sprite->addKeyframe(PIPE_IDLE, idleTexCoords);
    sprite->setAnimationSpeed(PIPE_BUSY, busyKeyFramesPerSecond);
    sprite->addKeyframe(PIPE_BUSY, busyTexCoords);
    sprite->addKeyframe(PIPE_BUSY, idleTexCoords);
    sprite->changeAnimation(PIPE_IDLE);
}

void PipeSegment::setActive(bool active)
{
    sprite->changeAnimation(active ? PIPE_BUSY : PIPE_IDLE);
}

void PipeSegment::update(int deltaTime)
{
    sprite->update(deltaTime);
}

void PipeSegment::render()
{
    sprite->render();
}