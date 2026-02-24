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
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


Player::Player()
{
	sprite = NULL;
	map = NULL;
}

Player::~Player()
{
	if (sprite != NULL)
		delete sprite;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;	// El jugador no está saltando
	// Cargamos el spritesheet del jugador, y creamos el sprite a partir de ese spritesheet
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	/*
		- El spritesheet mide 128x128
		- Cada Sprite mide 32x32
		Por eso paso el tamaño del sprite y el tamaño de cada frame 32/128 = 0.25 junto a la referencia del spritesheet
	*/
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	// Establezco el número de animaciones que tendrá el sprite
	sprite->setNumberAnimations(4);
		/*
			Por cada animación le paso el nombre, la velocidad y luego añado los frames que la componen
		*/
		// Animación de quieto mirando a la izquierda
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		// Animación de quieto mirando a la derecha
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		// Animación de movimiento hacia la izquierda
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		// Animación de movimiento hacia la derecha
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
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
			posPlayer.x -= 2;
			// Si al desplazar se detecta colisión vuelvo a la posición anterior y mantengo el sprite en la animación de quieto mirando a la izquierda
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	// Con la flecha derecha hago exactamente lo mismo
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
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
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	// Actualizo la posición del sprite con la posición del jugador
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	// En el render simplemente renderizo el sprite en la posición actual del jugador ya que el resto ya se ha calculado en el update
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




