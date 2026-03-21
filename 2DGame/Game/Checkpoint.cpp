#include "Checkpoint.h"

Checkpoint::Checkpoint() : Entity(Type::CHECKPOINT)
{
    activated = false;
    cooldownTimer = 0.f;
}

void Checkpoint::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
    Entity::init(tileMapPos, shaderProgram, "images/checkpoint.png", glm::vec2(32.f, 96.f), glm::vec2(1.f, 1.f), c);
}

void Checkpoint::update(int deltaTime)
{
    if (cooldownTimer > 0.f)
        cooldownTimer = max(0.f, cooldownTimer - (float)deltaTime);
}

void Checkpoint::activate()
{
    activated = true;
    cooldownTimer = COOLDOWN_DURATION;
}