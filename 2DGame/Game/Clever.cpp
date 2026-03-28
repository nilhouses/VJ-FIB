#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Clever.h"
#include "Game.h"


// Definimos 4 tipos de animaciones para el Clever
enum CleverAnims
{
    WALK_RIGHT, TUNNEL_ENTER_BOTTOM, WALK_LEFT, TUNNEL_LEAVE_BOTTOM, DIE, TUNNEL_ENTER_TOP, CLIMB, TUNNEL_LEAVE_TOP, FALL, START, NUM_ANIMS
};


Clever::Clever() : Enemy(EnemyType::CLEVER)
{

}

Clever::~Clever()
{
    if (sprite != NULL)
        delete sprite;
}

void Clever::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool movingRight)
{
    // Inicializar los atributos de la Entity
    Entity::init(tileMapPos, shaderProgram, "images/CleverEnemy.png", glm::ivec2(32, 64), glm::vec2(1.f / 16.f, 1.f / 5.f), c);

    // Atributos caracter�sticos del Clever
    dying = false;
    deathTimer = 0.f;

    // Configuración de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(WALK_RIGHT, 8);
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.f / 16.f, 0.f));
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(1.f / 16.f, 0.f));
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(2.f / 16.f, 0.f));
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(3.f / 16.f, 0.f));

    sprite->setAnimationSpeed(TUNNEL_ENTER_BOTTOM, 16);
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(5.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(6.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(7.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(8.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(9.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(10.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(11.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(12.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(13.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(14.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(15.f / 16.f, 0.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(5.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(6.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(7.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(8.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(9.f / 16.f, 1.f / 5.f));

    sprite->setAnimationSpeed(WALK_LEFT, 8);
    sprite->addKeyframe(WALK_LEFT, glm::vec2(0.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(WALK_LEFT, glm::vec2(1.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(WALK_LEFT, glm::vec2(2.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(WALK_LEFT, glm::vec2(3.f / 16.f, 1.f / 5.f));

    sprite->setAnimationSpeed(TUNNEL_LEAVE_BOTTOM, 14);
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(10.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(11.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(12.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(13.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(14.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(15.f / 16.f, 1.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(5.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(6.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(7.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(8.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(9.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(10.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(11.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(12.f / 16.f, 2.f / 5.f));

    sprite->setAnimationSpeed(DIE, 4);
    sprite->addKeyframe(DIE, glm::vec2(0.f, 2.f / 5.f));
    sprite->addKeyframe(DIE, glm::vec2(1.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(DIE, glm::vec2(2.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(DIE, glm::vec2(3.f / 16.f, 2.f / 5.f));

    sprite->setAnimationSpeed(TUNNEL_ENTER_TOP, 10);
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(14.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(15.f / 16.f, 2.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(5.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(6.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(7.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(8.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(9.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(10.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(11.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(12.f / 16.f, 3.f / 5.f));

    sprite->setAnimationSpeed(CLIMB, 12);
    sprite->addKeyframe(CLIMB, glm::vec2(0.f, 3.f / 5.f));
    sprite->addKeyframe(CLIMB, glm::vec2(1.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(CLIMB, glm::vec2(2.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(CLIMB, glm::vec2(3.f / 16.f, 3.f / 5.f));

    sprite->setAnimationSpeed(TUNNEL_LEAVE_TOP, 8);
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(13.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(14.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(15.f / 16.f, 3.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(10.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(11.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(12.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(13.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(14.f / 16.f, 4.f / 5.f));

    sprite->setAnimationSpeed(FALL, 8);
    sprite->addKeyframe(FALL, glm::vec2(0.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(FALL, glm::vec2(1.f / 16.f, 4.f / 5.f));

    sprite->setAnimationSpeed(START, 6);
    sprite->addKeyframe(START, glm::vec2(2.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(3.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(4.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(5.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(6.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(7.f / 16.f, 4.f / 5.f));
    sprite->addKeyframe(START, glm::vec2(8.f / 16.f, 4.f / 5.f));

    this->movingRight = movingRight;
    startAnimTimer = 1000.0f;
    sprite->changeAnimation(START);
    setSpeed(1);
    cam = c;
}

void Clever::render()
{
    if (visible) sprite->render();
}

bool Clever::centeredOn(const glm::vec4& bbox) const
{
    float centerX = pos.x + size.x * 0.5f;
    float centerY = pos.y + size.y * 0.5f;
    return centerX >= bbox.x && centerX <= bbox.x + bbox.z &&
        centerY >= bbox.y && centerY <= bbox.y + bbox.w;
}

bool static overlaps(const glm::vec4& a, const glm::vec4& b) {
    return !(a.x + a.z < b.x || b.x + b.z < a.x || a.y + a.w < b.y || b.y + b.w < a.y);
}

// Función auxiliar para saber si puede subir por una pipe
pair<Pipe*, int> Clever::getPipeEntryAt() const
{
    glm::vec4 myBox = getBoundingBox();
    for (Pipe* p : pipes) {
        glm::vec4 b0 = p->getEndBoundingBox(0);
        glm::vec4 b1 = p->getEndBoundingBox(1);
        if (overlaps(myBox, b0) && centeredOn(b0)) return { p, 0 };
        if (overlaps(myBox, b1) && centeredOn(b1)) return { p, 1 };
    }
    return { nullptr, -1 };
}


void Clever::update(int deltaTime)
{
    sprite->update(deltaTime);

    if (startAnimTimer > 0) {
        if (sprite->animation() == DIE) { startAnimTimer = 0; return; }
        startAnimTimer -= deltaTime;
        if (startAnimTimer <= 0) {
            movingRight = (playerTarget->getPosition().x > pos.x);
            sprite->changeAnimation(movingRight ? WALK_RIGHT : WALK_LEFT);
        }
        return;
    }

    if (isDying()) {
        deathTimer += deltaTime;
        if (deathTimer >= deathDuration) this->deactivate();
        return;
    }

    // Transición túnel
    if (inTunnel) {
        if (!tunnelTeleported) { // Animación de entrada
            if (sprite->isLastKeyframe()) {
                setVisible(false);
                Tunnel* exitTunnel = static_cast<Tunnel*>(currentTunnel->getConnectedTo());
                glm::vec2 exitPos = exitTunnel->getPosition();
                pos = glm::ivec2((int)exitPos.x, (int)exitPos.y - 1);
                lastUsedTunnel = exitTunnel;
                tunnelTeleported = true;
                setVisible(true);
                if (exitTunnel->getUp()) sprite->changeAnimation(TUNNEL_LEAVE_TOP);
                else sprite->changeAnimation(TUNNEL_LEAVE_BOTTOM);
            }
            else {
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
                return;
            }
        } // Animación de salida        
        else if (tunnelTeleported) {
            if (sprite->isLastKeyframe()) {
                hurts = true;
                inTunnel = false;
                tunnelTeleported = false;
                currentTunnel = nullptr;
                movingRight = (playerTarget->getPosition().x > pos.x);
                sprite->changeAnimation(movingRight ? WALK_RIGHT : WALK_LEFT);
            }
        }
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
        return;
    }

    // Transición tubo
    if (inPipe) {
        if (currentPipe->isTransitComplete()) {
            glm::vec2 exitPos = currentPipe->getExitPosition(size.y);
            pos = glm::ivec2((int)exitPos.x, (int)exitPos.y);

            glm::vec2 endCenter = glm::vec2(exitPos.x + map->getTileSize() * 0.5f, exitPos.y + map->getTileSize() * 0.5f);
            bool exitingUp = currentPipe->isExitingUp();

            if (cam->isVisible(endCenter + glm::vec2(0, 200)) ||
                cam->isVisible(endCenter + glm::vec2(0, -200)) ||
                cam->isVisible(endCenter + glm::vec2(200, 0)) ||
                cam->isVisible(endCenter + glm::vec2(-200, 0))
                ) SoundManager::instance().playSound("pipe_out", 0.05f);

            setVisible(true);
            inPipe = false;
            currentPipe = nullptr;
            verticalCooldown = VERTICAL_COOLDOWN;
            verticalCooldown = VERTICAL_COOLDOWN;

            if (exitingUp) { // Animación de salida del tubo (como START)
                startAnimTimer = 1000.0f;
                sprite->changeAnimation(START);
            }
            else { // Salida por debajo del tubo, animación de caída breve
                startAnimTimer = 400.0f;
                sprite->changeAnimation(FALL);
            }
        }
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
        return;
    }
    if (verticalCooldown > 0) verticalCooldown -= deltaTime;

    glm::ivec2 playerPos = playerTarget->getPosition();
    int playerBottomY = playerPos.y + playerTarget->getSize().y;
    int cleverBottomY = pos.y + size.y;

    bool canClimbUp = map->collisionLadderUp(pos, size);
    bool canClimbDown = map->collisionLadderDown(pos, size);

    // Pipes disponibles
    pair<Pipe*, int> pipeEntry = getPipeEntryAt();
    Pipe* foundPipe = pipeEntry.first;
    int pipeEnd = pipeEntry.second;
    bool canPipeUp = false;
    bool canPipeDown = false;
    if (foundPipe != nullptr) {
        int exitEnd = (pipeEnd == 0) ? 1 : 0;
        glm::vec2 exitPos = foundPipe->getExitPosition(size.y, pipeEnd);
        float currentY = (float)(pos.y + size.y);
        float exitY = exitPos.y + size.y;
        canPipeUp = (exitY < currentY) && !foundPipe->isOccupied() && verticalCooldown <= 0;
        canPipeDown = (exitY > currentY) && !foundPipe->isOccupied() && verticalCooldown <= 0;
    }

    // 1. Movimiento vertical
    isClimbing = false;

    if (wasClimbing && ladderDirection != 0) {
        bool canContinue = (ladderDirection < 0) ? canClimbUp : canClimbDown;
        if (canContinue) {
            centerX();
            pos.y += ladderDirection * speed;
            isClimbing = true;
            if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
        }
        else {
            ladderDirection = 0;
            verticalCooldown = VERTICAL_COOLDOWN;
        }
    }

    if (!isClimbing && playerBottomY != cleverBottomY && verticalCooldown <= 0) {
        if (playerBottomY < cleverBottomY) {   // Subir
            if (canClimbUp) {
                ladderDirection = -1;
                centerX();
                pos.y -= speed;
                isClimbing = true;
                if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
            }
            else if (canPipeUp) {
                currentPipe = foundPipe;
                inPipe = true;
                setVisible(false);
                foundPipe->startTransit(pipeEnd, false);

                // Sonido de entrada en la pipe si está cerca del jugador
                glm::vec2 pPos = playerTarget->getPosition();
                glm::vec4 b = foundPipe->getEndBoundingBox(pipeEnd);
                glm::vec2 endCenter(b.x + b.z * 0.5f, b.y + b.w * 0.5f);
                if (cam->isVisible(endCenter + glm::vec2(0, 200)) ||
                    cam->isVisible(endCenter + glm::vec2(0, -200)) ||
                    cam->isVisible(endCenter + glm::vec2(200, 0)) ||
                    cam->isVisible(endCenter + glm::vec2(-200, 0))
                    ) SoundManager::instance().playSound("pipe_in", 0.05f);

                sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
                return;
            }
        }
        else if (playerBottomY > cleverBottomY) {   // Bajar
            if (canClimbDown) {
                ladderDirection = +1;
                centerX();
                pos.y += speed;
                isClimbing = true;
                if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
            }
            else if (canPipeDown) {
                currentPipe = foundPipe;
                inPipe = true;
                setVisible(false);
                foundPipe->startTransit(pipeEnd, false);

                // Sonido de entrada en la pipe si está cerca del jugador
                glm::vec2 pPos = playerTarget->getPosition();
                glm::vec4 b = foundPipe->getEndBoundingBox(pipeEnd);
                glm::vec2 endCenter(b.x + b.z * 0.5f, b.y + b.w * 0.5f);
                if (cam->isVisible(endCenter + glm::vec2(0, 200)) ||
                    cam->isVisible(endCenter + glm::vec2(0, -200)) ||
                    cam->isVisible(endCenter + glm::vec2(200, 0)) ||
                    cam->isVisible(endCenter + glm::vec2(-200, 0))
                    ) SoundManager::instance().playSound("pipe_in", 0.05f);

                sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
                return;
            }
        }
    }

    // 2. Movimiento horizontal
    if (!isClimbing) {
        int mapWidth = map->getMapSize().x * map->getTileSize();
        if (movingRight) {
            if (sprite->animation() != WALK_RIGHT) sprite->changeAnimation(WALK_RIGHT);
            glm::ivec2 nextPos(pos.x + speed, pos.y);
            if (!map->collisionMoveRightEnemy(nextPos, size) && nextPos.x + size.x < mapWidth)
                pos.x += speed;
            else changeDirection();
        }
        else {
            if (sprite->animation() != WALK_LEFT) sprite->changeAnimation(WALK_LEFT);
            glm::ivec2 nextPos(pos.x - speed, pos.y);
            if (!map->collisionMoveLeftEnemy(nextPos, size) && nextPos.x > 0)
                pos.x -= speed;
            else changeDirection();
        }
    }

    // 3. Gravedad
    if (!isClimbing) {
        if (!onGround) {
            int oldY = pos.y;
            pos.y += fallStep;
            map->collisionMoveDown(pos, size, &pos.y, fallStep, 6);
            onGround = (pos.y == oldY);
        }
        bool justLanded = (wasInAir && onGround) || (wasClimbing && !isClimbing);
        if (!onGround && sprite->animation() != FALL) sprite->changeAnimation(FALL);
        else if (justLanded) {
            movingRight = (playerTarget->getPosition().x > pos.x);
            sprite->changeAnimation(movingRight ? WALK_RIGHT : WALK_LEFT);
        }

        wasInAir = !onGround;
        onGround = false;
    }
    else {
        wasInAir = false;
        onGround = false;
    }
    wasClimbing = isClimbing;

    sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Clever::changeDirection() {
    if (isClimbing) return;
    movingRight = !movingRight;
    if (movingRight) sprite->changeAnimation(WALK_RIGHT);
    else sprite->changeAnimation(WALK_LEFT);
}

void Clever::notifyTunnelEntry(Tunnel* t)
{
    if (inTunnel) return;
    inTunnel = true;

    // Ignora el túnel hasta que se haya alejado lo suficiente del último túnel usado,
    if (lastUsedTunnel != nullptr) {
        glm::vec2 exitPos = lastUsedTunnel->getPosition();
        float dx = pos.x - exitPos.x;
        float dy = pos.y - exitPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < TUNNEL_CLEAR_DISTANCE) { inTunnel = false; return; } // Usamos distancia euclidiana para evitar que el clever tenga que alejarse exactamente en horizontal o vertical del túnel
        else lastUsedTunnel = nullptr;
    }

    Tunnel* exit = static_cast<Tunnel*>(t->getConnectedTo());
    if (!exit) { inTunnel = false; return; }
    bool shouldEnter = false;

    // Tiene que subir si el túnel va hacia arriba y el jugador está más arriba
    if (!t->getUp()) {
        int playerBottomY = playerTarget->getPosition().y + playerTarget->getSize().y;
        int cleverBottomY = pos.y + size.y;
        shouldEnter = (playerBottomY < cleverBottomY);
    }
    else { // o bajar si el túnel va hacia abajo y el jugador está más abajo
        int playerBottomY = playerTarget->getPosition().y + playerTarget->getSize().y;
        int cleverBottomY = pos.y + size.y;
        shouldEnter = (playerBottomY > cleverBottomY);
    }

    if (!shouldEnter) { inTunnel = false; return; }

    centerX();
    centerY();
    hurts = false;
    currentTunnel = t;
    inTunnel = true;
    tunnelTeleported = false;

    // Sonido de entrada
    glm::vec4 b = t->getBoundingBox();
    glm::vec2 entryCenter(b.x + b.z * 0.5f, b.y + b.w * 0.5f);

    if (cam->isVisible(entryCenter + glm::vec2(0, 200))||
        cam->isVisible(entryCenter + glm::vec2(0, -200))||
        cam->isVisible(entryCenter + glm::vec2(200, 0))||
        cam->isVisible(entryCenter + glm::vec2(-200, 0))
    ) SoundManager::instance().playSound("tunnelSteps", 0.2f); // Más flojo que el player para quitarle algo de importancia

    if (currentTunnel->getUp()) sprite->changeAnimation(TUNNEL_ENTER_TOP);
    else sprite->changeAnimation(TUNNEL_ENTER_BOTTOM);
}

void Clever::die()
{
    if (isDying()) return;
    dying = true;
    sprite->changeAnimation(DIE);
    std::cout << "RIP Clever" << std::endl;
	SoundManager::instance().playSound("pigDeath", 0.1f);
    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}
