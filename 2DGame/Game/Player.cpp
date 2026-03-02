#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4	// Velocidad del salto del jugador, cuanto más alto más lento será el salto
#define JUMP_HEIGHT 96		// Altura máxima del salto del jugador
#define FALL_STEP 4			// Velocidad de caída del jugador
#define SPEED 2

// Definimos 4 tipos de animaciones para el jugador
enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, ENTERING_DOOR, EXITING_DOOR, NUM_ANIMS
};


Player::Player() : Entity(Type::PLAYER)
{
	map = NULL;
}

Player::~Player()
{
	if (sprite != NULL)
		delete sprite;
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/bub.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Inicializar los atributos del Player
	bJumping = false;
	jumpAngle = 0;
	startY = 0;
	blockedInput = false;

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 20 );
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 20);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(ENTERING_DOOR, 20);
	sprite->addKeyframe(ENTERING_DOOR, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(ENTERING_DOOR, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(ENTERING_DOOR, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(ENTERING_DOOR, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(EXITING_DOOR, 20);
	sprite->addKeyframe(EXITING_DOOR, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(EXITING_DOOR, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(EXITING_DOOR, glm::vec2(0.75f, 0.5f));

	sprite->changeAnimation(STAND_RIGHT);
}


// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	if (!blockedInput) {

		// Con la flecha hacia arriba el personaje subirá si existe una escalera en esa posición
		if (Game::instance().getKey(GLFW_KEY_UP)) {
			if (map->collisionLadderUp(pos, glm::ivec2(32, 32))) {
				// TODO: Aplicar la nueva animación de subir escaleras
				pos.y -= SPEED;

				bJumping = false;
			}
			// TEMPORAL: Si no hay escalera, el personaje se quedará quieto mirando hacia el lado que corresponda
			else {
				if (sprite->animation() == MOVE_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == MOVE_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		// Con la flecha hacia abajo el personaje bajará si existe una escalera en esa posición
		else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
			if (map->collisionLadderDown(pos, glm::ivec2(32, 32))) {
				// TODO: Aplicar la nueva animación de bajar escaleras
				pos.y += SPEED;
				bJumping = false;
			} 
			else {
				if (sprite->animation() == MOVE_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == MOVE_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}
		// Si la flecha izquierda está pulsada
		else if(Game::instance().getKey(GLFW_KEY_LEFT))
		{
			// Si la animación actual no es moverse a la izquierda, cambio la animación a mover a la izquierda y le sumo desplazamiento
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			incrLeft();
			// Si detecto colisión o se sale del mapa
			if (map->collisionMoveLeft(pos, glm::ivec2(32, 32)) || pos.x < 0.f)
			{
				incrRight();
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		// Con la flecha derecha hago exactamente lo mismo
		else if(Game::instance().getKey(GLFW_KEY_RIGHT))
		{
			// Si la animación actual no es moverse a la derecha, cambio la animación a mover a la izquierda y le sumo desplazamiento
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			incrRight();
			// Si detecto colisión o se sale del mapa
			if (map->collisionMoveRight(pos, glm::ivec2(32, 32)) || pos.x > ((map->getMapSize().x - 1) * map->getTileSize()))
			{
				incrLeft();
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		// Si ninguna de las flechas está pulsada entonces dejo el personaje quieto mirando hacia el lado que corresponda
		else
		{
			if(sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if(sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		// Si está saltando
		if(bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if(jumpAngle == 180) {
				bJumping = false;
				pos.y = startY;
			}
			else
			{
				pos.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if(jumpAngle > 90)
					bJumping = !map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, FALL_STEP);
			}
		}
		else if (!map->collisionLadderUp(pos, glm::ivec2(32, 32)) && !map->collisionLadderDown(pos, glm::ivec2(32, 32)))
		{
			pos.y += FALL_STEP;
			if(map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y, FALL_STEP))
			{
				if(Game::instance().getKey(GLFW_KEY_SPACE))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = pos.y;
				}
			}
		}
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Player::setAnimation(const string& anim)
{
	if (anim == "STAND_LEFT")
		sprite->changeAnimation(STAND_LEFT);
	else if (anim == "STAND_RIGHT")
		sprite->changeAnimation(STAND_RIGHT);
	else if (anim == "MOVE_LEFT")
		sprite->changeAnimation(MOVE_LEFT);
	else if (anim == "MOVE_RIGHT")
		sprite->changeAnimation(MOVE_RIGHT);
	else if (anim == "ENTERING_DOOR")
		sprite->changeAnimation(ENTERING_DOOR);
	else if (anim == "EXITING_DOOR")
		sprite->changeAnimation(EXITING_DOOR);
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

int Player::getSpeed() {
	return SPEED;
}


void Player::incrRight()
{
	pos.x += SPEED;
}

void Player::incrLeft()
{
	pos.x -= SPEED;
}

void Player::incrUp(int px)
{
	pos.y -= (px + FALL_STEP); // Evitar gravedad
}