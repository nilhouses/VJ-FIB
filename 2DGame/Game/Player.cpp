#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4	// Velocidad del salto del jugador, cuanto más alto más lento será el salto
#define JUMP_HEIGHT 96		// Altura máxima del salto del jugador
#define FALL_STEP 4			// Velocidad de caída del jugador

// Definimos 4 tipos de animaciones para el jugador
enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, NUM_ANIMS
};


Player::Player()
{
	map = NULL;
}

Player::~Player()
{
	if (sprite != NULL)
		delete sprite;
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/bub.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f));

	// Inicializar los atributos del Player
	bJumping = false;
	jumpAngle = 0;
	startY = 0;

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));

	sprite->changeAnimation(STAND_LEFT);
}

// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	// Si la flecha izquierda está pulsada
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		// Si la animación actual no es movershe a la izquierda, cambio la animación a mover a la izquierda y le sumo desplazamiento
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
			pos.x -= 2;
			// Si al desplazar se detecta colisión vuelvo a la posición anterior y mantengo el sprite en la animación de quieto mirando a la izquierda
		if(map->collisionMoveLeft(pos, glm::ivec2(32, 32)))
		{
			pos.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	// Con la flecha derecha hago exactamente lo mismo
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		pos.x += 2;
		if(map->collisionMoveRight(pos, glm::ivec2(32, 32)))
		{
			pos.x -= 2;
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
				bJumping = !map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y);
		}
	}
	else
	{
		pos.y += FALL_STEP;
		if(map->collisionMoveDown(pos, glm::ivec2(32, 32), &pos.y))
		{
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = pos.y;
			}
		}
	}
	// Actualizo la posición del sprite con la posición del jugador
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}
