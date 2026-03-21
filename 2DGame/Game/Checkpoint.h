#ifndef _CHECKPOINT_INCLUDE
#define _CHECKPOINT_INCLUDE

#include "Entity.h"

class Checkpoint : public Entity
{
public:
    Checkpoint();
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c);
    void update(int deltaTime) override;

    bool isReady() const { return cooldownTimer <= 0.f; }
    bool isActivated() const { return activated; }
    void activate();

    float getCooldown() const { return cooldownTimer; }

private:
    bool activated;
    float cooldownTimer;
    static constexpr float COOLDOWN_DURATION = 10000.f; // 10 segundos, lo cambiamos a lo que nos convenga
};

#endif // _CHECKPOINT_INCLUDE