#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Clever.h"
#include "Game.h"

// Definimos 4 tipos de animaciones para el Clever
enum CleverAnims
{
    MOVE_LEFT, MOVE_RIGHT, CLIMB, DIE, NUM_ANIMS
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
    Entity::init(tileMapPos, shaderProgram, "images/clever.png", glm::ivec2(32, 64), glm::vec2(0.25f, 0.25f), c);

    // Atributos caracter�sticos del Clever
    dying = false;
    deathTimer = 0.f;

    // Configuraci�n de animaciones
    sprite->setNumberAnimations(NUM_ANIMS);

    sprite->setAnimationSpeed(MOVE_LEFT, 20);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.50f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.25f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 20);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50f, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75f, 0.f));

    sprite->setAnimationSpeed(CLIMB, 15);
    sprite->addKeyframe(CLIMB, glm::vec2(0.f, 0.75f));
    sprite->addKeyframe(CLIMB, glm::vec2(0.25f, 0.75f));

    sprite->setAnimationSpeed(DIE, 3);
    sprite->addKeyframe(DIE, glm::vec2(0.5f, 0.75f));
    sprite->addKeyframe(DIE, glm::vec2(0.75f, 0.75f));

    this->movingRight = movingRight;
    sprite->changeAnimation(movingRight ? MOVE_RIGHT : MOVE_LEFT);
    setSpeed(2);
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

bool overlaps(const glm::vec4& a, const glm::vec4& b) {
    return !(a.x + a.z < b.x || b.x + b.z < a.x || a.y + a.w < b.y || b.y + b.w < a.y);
}

// Función auxiliar para saber si puede subir por una escalera
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

    if (isDying()) {
        deathTimer += deltaTime;
        if (deathTimer >= deathDuration) this->deactivate();
        return;
    }

    // Transición túnel
    if (inTunnel) {
        tunnelTimer -= deltaTime;

        // Tp a la salida (Ya se ha terminado la animación de entrada      
        if (tunnelTimer <= TUNNEL_DURATION/2.0f && !tunnelTeleported) {
            // Tp a la salida
            Tunnel* exitTunnel = static_cast<Tunnel*>(currentTunnel->getConnectedTo());
            glm::vec2 exitPos = exitTunnel->getPosition();
            pos = glm::ivec2((int)exitPos.x, (int)exitPos.y);
            lastUsedTunnel = exitTunnel;
            tunnelTeleported = true;
            setVisible(true);
			// [TODO]
            // if (currentTunnel->getUp())
            //      if (sprite->animation() != TUNNEL_LEAVE_TOP) sprite->changeAnimation(TUNNEL_LEAVE_TOP);
            // else
            //      if (sprite->animation() != TUNNEL_LEAVE_BOTTOM) sprite->changeAnimation(TUNNEL_LEAVE_BOTTOM);
        }
        if (tunnelTimer <= 0.f) {
            hurts = true;
            inTunnel = false;
            tunnelTeleported = false;
            currentTunnel = nullptr;
            movingRight = (playerTarget->getPosition().x > pos.x);
            sprite->changeAnimation(movingRight ? MOVE_RIGHT : MOVE_LEFT);
		}

        sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
        return;
    }

    // Transición tubo
    if (inPipe) {
        if (currentPipe->isTransitComplete()) {
            glm::vec2 exitPos = currentPipe->getExitPosition(size.y);
            pos = glm::ivec2(exitPos.x, exitPos.y);

            float tileSize = (float)map->getTileSize();
            glm::vec2 endCenter(exitPos.x + tileSize * 0.5f, exitPos.y + tileSize * 0.5f);
            glm::vec2 pPos = playerTarget->getPosition();
            if (abs(pPos.x - endCenter.x) < PLAYER_SOUND_RANGE_X && abs(pPos.y - endCenter.y) < PLAYER_SOUND_RANGE_Y) SoundManager::instance().playSound("pipe_out", 0.05f);

            setVisible(true);
            inPipe = false;
            currentPipe = nullptr;               
            verticalCooldown = VERTICAL_COOLDOWN;
            movingRight = (playerTarget->getPosition().x > pos.x);
            if (movingRight) sprite->changeAnimation(MOVE_RIGHT);
            else sprite->changeAnimation(MOVE_LEFT);
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
        canPipeUp = (exitY < currentY) && !foundPipe->isOccupied();
        canPipeDown = (exitY > currentY) && !foundPipe->isOccupied();
    }

    // 1. Movimiento vertical
    isClimbing = false;
    if (playerBottomY != cleverBottomY) {
        if (playerBottomY < cleverBottomY) {   // Subir
            if (canClimbUp) {
                center();
                pos.y -= speed;
                isClimbing = true;  
                if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
            }
            else if (canPipeUp && verticalCooldown <= 0) {
                currentPipe = foundPipe;
                inPipe = true;
                setVisible(false);
                foundPipe->startTransit(pipeEnd,false);

				// Sonido de entrada en la pipe si está cerca del jugador
                glm::vec2 pPos = playerTarget->getPosition();
                glm::vec4 b = foundPipe->getEndBoundingBox(pipeEnd);
                glm::vec2 endCenter(b.x + b.z * 0.5f, b.y + b.w * 0.5f);
                if (abs(pPos.x - endCenter.x) < PLAYER_SOUND_RANGE_X && abs(pPos.y - endCenter.y) < PLAYER_SOUND_RANGE_Y) SoundManager::instance().playSound("pipe_in", 0.05f);

                sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
                return;
            }
        }
        else if (playerBottomY > cleverBottomY) {   // Bajar
            if (canClimbDown) {
                center();
                pos.y += speed;
                isClimbing = true;
                if (sprite->animation() != CLIMB) sprite->changeAnimation(CLIMB);
            }
            else if (canPipeDown && verticalCooldown <= 0) {
                currentPipe = foundPipe;
                inPipe = true;
                setVisible(false);
                foundPipe->startTransit(pipeEnd, false);
                // Sonido de entrada en la pipe si está cerca del jugador
                glm::vec2 pPos = playerTarget->getPosition();
                glm::vec4 b = foundPipe->getEndBoundingBox(pipeEnd);
                glm::vec2 endCenter(b.x + b.z * 0.5f, b.y + b.w * 0.5f);
                
                if (abs(pPos.x - endCenter.x) < PLAYER_SOUND_RANGE_X && abs(pPos.y - endCenter.y) < PLAYER_SOUND_RANGE_Y) SoundManager::instance().playSound("pipe_in", 0.05f);

                sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
                return;
            }
        }
    }

    // 2. Movimiento horizontal
    if (!isClimbing) {
        int mapWidth = map->getMapSize().x * map->getTileSize();
        if (movingRight) {
            if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
            glm::ivec2 nextPos(pos.x + speed, pos.y);
            if (!map->collisionMoveRightEnemy(nextPos, size) && nextPos.x + size.x < mapWidth)
                pos.x += speed;
            else changeDirection();
        }
        else {
            if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
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
            map->collisionMoveDown(pos, size, &pos.y, fallStep);
            onGround = (pos.y == oldY);
        }
        bool justLanded = (wasInAir && onGround) || (wasClimbing && !isClimbing);
        if (justLanded) movingRight = (playerPos.x > pos.x);

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
    if (movingRight) sprite->changeAnimation(MOVE_RIGHT);
    else sprite->changeAnimation(MOVE_LEFT);
}

void Clever::notifyTunnelEntry(Tunnel * t)
{  
    if (inTunnel) return;

	// Ignora el túnel hasta que se haya alejado lo suficiente del último túnel usado,
    if (lastUsedTunnel != nullptr) {
        glm::vec2 exitPos = lastUsedTunnel->getPosition();
        float dx = pos.x - exitPos.x;
        float dy = pos.y - exitPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);
		if (dist < TUNNEL_CLEAR_DISTANCE) return; // Usamos distancia euclidiana para evitar que el clever tenga que alejarse exactamente en horizontal o vertical del túnel
        else lastUsedTunnel = nullptr;
    }

    Tunnel* exit = static_cast<Tunnel*>(t->getConnectedTo());
    if (!exit) return;
    bool shouldEnter = false;

	// Tiene que subir si el túnel va hacia arriba y el jugador está más arriba
    if (!t->getUp()) {
        int playerBottomY = playerTarget->getPosition().y + playerTarget->getSize().y;
        int cleverBottomY = pos.y + size.y;
        shouldEnter = (playerBottomY < cleverBottomY);
    } else { // o bajar si el túnel va hacia abajo y el jugador está más abajo
        int playerBottomY = playerTarget->getPosition().y + playerTarget->getSize().y;
        int cleverBottomY = pos.y + size.y;
        shouldEnter = (playerBottomY > cleverBottomY);
    }

    if (!shouldEnter) return;
    hurts = false;
    currentTunnel = t;
    inTunnel = true;
    tunnelTimer = TUNNEL_DURATION;
    setVisible(false);
    // [TODO] Animación de entrada
    // if (currentTunnel->getUp())
    //      if (sprite->animation() != TUNNEL_ENTER_TOP) sprite->changeAnimation(TUNNEL_ENTER_TOP);
    // else
    //      if (sprite->animation() != TUNNEL_ENTER_BOTTOM) sprite->changeAnimation(TUNNEL_ENTER_BOTTOM);

}

void Clever::die()
{
    if (isDying()) return;
    dying = true;
    sprite->changeAnimation(DIE);
    std::cout << "RIP Clever" << std::endl;
    // En el update se desactivar� la entidad cuando acabe la animaci�n de explosi�n
}

