#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Room.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4	// Velocidad del salto del jugador, cuanto más alto más lento será el salto
#define JUMP_HEIGHT 96		// Altura máxima del salto del jugador
#define FALL_STEP 4			// Velocidad de caída del jugador
#define SPEED 2 			// Velocidad de movimiento del jugador
#define PICK_ITEM_TIMER 500 // Duración de la animación de recoger un item (ms)

// Definimos tipos de animaciones para el jugador
enum PlayerAnims
{
	IDLE, WALK_LEFT, WALK_RIGHT, WEAPON_IDLE, GUN_WALK_LEFT, GUN_WALK_RIGHT, FALL, PUSH_LEFT, PUSH_RIGHT, DIE, START, CLIMB, OPEN_AND_ENTER, PICK_ITEM, ENTER, TUNNEL_ENTER_BOTTOM, TUNNEL_ENTER_TOP, TUNNEL_LEAVE_TOP, TUNNEL_LEAVE_BOTTOM, NUM_ANIMS
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
	Entity::init(tileMapPos, shaderProgram, "images/sprite_sheet_animations.png", glm::ivec2(32, 64), glm::vec2(1.f/16.f, 1.f/12.f), c);

	// Inicializar los atributos del Player
	bJumping = false;
	jumpAngle = 0;
	startY = 0;
	blockedInput = false;
	itemPickTimer = 0;
	speedBoostTimer = 0;
	this->shaderProgram = &shaderProgram;
	this->cameraPtr = c;

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 4);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 16.f, 0.f));

	sprite->setAnimationSpeed(WALK_LEFT, 8);
	sprite->addKeyframe(WALK_LEFT, glm::vec2(3.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(4.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(5.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(6.f / 16.f, 0.f));

	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(8.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(9.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(10.f / 16.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(11.f / 16.f, 0.f));

	sprite->setAnimationSpeed(WEAPON_IDLE, 8);
	sprite->addKeyframe(WEAPON_IDLE, glm::vec2(0.f, 1.f / 12.f));
	sprite->addKeyframe(WEAPON_IDLE, glm::vec2(1.f / 16.f, 1.f / 12.f));

	sprite->setAnimationSpeed(GUN_WALK_LEFT, 8);
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(3.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(4.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(5.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(6.f / 16.f, 1.f / 12.f));

	sprite->setAnimationSpeed(GUN_WALK_RIGHT, 8);
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(8.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(9.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(10.f / 16.f, 1.f / 12.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(11.f / 16.f, 1.f / 12.f));

	sprite->setAnimationSpeed(FALL, 20);
	sprite->addKeyframe(FALL, glm::vec2(0.f, 2.f / 12.f));
	sprite->addKeyframe(FALL, glm::vec2(1.f / 16.f, 2.f / 12.f));

	sprite->setAnimationSpeed(PUSH_LEFT, 8);
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(3.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(4.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(5.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(6.f / 16.f, 2.f / 12.f));

	sprite->setAnimationSpeed(PUSH_RIGHT, 8);
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(8.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(9.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(10.f / 16.f, 2.f / 12.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(11.f / 16.f, 2.f / 12.f));

	sprite->setAnimationSpeed(DIE, 4);
	sprite->addKeyframe(DIE, glm::vec2(0.f, 3.f / 12.f));
	sprite->addKeyframe(DIE, glm::vec2(1.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(DIE, glm::vec2(2.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(DIE, glm::vec2(3.f / 16.f, 3.f / 12.f));

	sprite->setAnimationSpeed(START, 8);
	sprite->addKeyframe(START, glm::vec2(5.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(6.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(7.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(8.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(9.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(10.f / 16.f, 3.f / 12.f));
	sprite->addKeyframe(START, glm::vec2(11.f / 16.f, 3.f / 12.f));

	sprite->setAnimationSpeed(CLIMB, 12);
	sprite->addKeyframe(CLIMB, glm::vec2(0.f, 4.f / 12.f));
	sprite->addKeyframe(CLIMB, glm::vec2(1.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(CLIMB, glm::vec2(2.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(CLIMB, glm::vec2(3.f / 16.f, 4.f / 12.f));

	sprite->setAnimationSpeed(OPEN_AND_ENTER, 17);
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(5.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(6.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(7.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(8.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(9.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(10.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(11.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(12.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(13.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(14.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(15.f / 16.f, 4.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(0.f, 5.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(1.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(2.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(3.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(4.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(5.f / 16.f, 5.f / 12.f));

	sprite->setAnimationSpeed(PICK_ITEM, 10);
	sprite->addKeyframe(PICK_ITEM, glm::vec2(7.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(8.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(9.f / 16.f, 5.f / 12.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(10.f / 16.f, 5.f / 12.f));

	sprite->setAnimationSpeed(ENTER, 9);
	sprite->addKeyframe(ENTER, glm::vec2(0.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(1.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(2.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(3.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(4.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(5.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(6.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(7.f / 16.f, 6.f / 12.f));
	sprite->addKeyframe(ENTER, glm::vec2(8.f / 16.f, 6.f / 12.f));

	sprite->setAnimationSpeed(TUNNEL_ENTER_BOTTOM, 16);
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(0.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(1.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(2.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(3.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(4.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(5.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(6.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(7.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(8.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(9.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(10.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(11.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(12.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(13.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(14.f / 16.f, 7.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_BOTTOM, glm::vec2(15.f / 16.f, 7.f / 12.f));

	sprite->setAnimationSpeed(TUNNEL_ENTER_TOP, 9);
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(0.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(1.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(2.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(3.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(4.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(5.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(6.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(7.f / 16.f, 8.f / 12.f));
	sprite->addKeyframe(TUNNEL_ENTER_TOP, glm::vec2(8.f / 16.f, 8.f / 12.f));

	sprite->setAnimationSpeed(TUNNEL_LEAVE_BOTTOM, 8);
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(0.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(1.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(2.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(3.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(4.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(5.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(6.f / 16.f, 9.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_BOTTOM, glm::vec2(7.f / 16.f, 9.f / 12.f));

	sprite->setAnimationSpeed(TUNNEL_LEAVE_TOP, 14);
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(0.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(1.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(2.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(3.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(4.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(5.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(6.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(7.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(8.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(9.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(10.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(11.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(12.f / 16.f, 10.f / 12.f));
	sprite->addKeyframe(TUNNEL_LEAVE_TOP, glm::vec2(13.f / 16.f, 10.f / 12.f));

	startAnimTimer = 750.0f;
	sprite->changeAnimation(START);
	blockInput();
}


// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (startAnimTimer > 0) {
		startAnimTimer -= deltaTime;
		if (startAnimTimer <= 0) unblockInput();
		return;
	}
	// Animación de recoger un item no permite hacer nada más
	if (sprite->animation() == PICK_ITEM) {
		itemPickTimer -= deltaTime;
		if (itemPickTimer <= 0) {
			unblockInput();
			(numBullets > 0) ? sprite->changeAnimation(WEAPON_IDLE) : sprite->changeAnimation(IDLE);
		}
	}

	// SpeedBoost
	if (speedBoostTimer > 0) {
		speedBoostTimer -= deltaTime;
		if (speedBoostTimer <= 0) {
			speedBoostTimer = 0;
			speedMultiplier = 1.f;
		}
	}
	
	// Disparos
	if (shootCooldown > 0) shootCooldown -= deltaTime;
	if (isShooting) {
		shootAnimTimer -= deltaTime;
		if (shootAnimTimer <= 0) {
			isShooting = false;
			unblockInput();
			(numBullets > 0) ? sprite->changeAnimation(WEAPON_IDLE) : sprite->changeAnimation(IDLE);
		}
	}

	bool inputDetected = true;
	
	if (!blockedInput) {

		if (Game::instance().getKey(GLFW_KEY_Q) && hasBullets()) {
			shoot();
			numBullets--;
			// [TODO] Animación de disparo y/o bloquear input temporalmente
			// blockInput();
			// sprite->changeAnimation(facingRight ? SHOOT_RIGHT : SHOOT_LEFT);
		}

		// Con la flecha hacia arriba el personaje subirá si existe una escalera en esa posición
		if (Game::instance().getKey(GLFW_KEY_UP)) {
			if (map->collisionLadderUp(pos, getSize())) {
				center();
				if (sprite->animation() != CLIMB)
					sprite->changeAnimation(CLIMB);
				sprite->setPaused(false);
				pos.y -= (int)(SPEED * speedMultiplier);
				bJumping = false;
			}
			else if (sprite->animation() != IDLE && sprite->animation() != WEAPON_IDLE)
				(numBullets > 0) ? sprite->changeAnimation(WEAPON_IDLE) : sprite->changeAnimation(IDLE);
		}
		// Con la flecha hacia abajo el personaje bajará si existe una escalera en esa posición
		else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
			if (map->collisionLadderDown(pos, getSize())) {
				center();
				if (sprite->animation() != CLIMB)
					sprite->changeAnimation(CLIMB);

				sprite->setPaused(false);
				pos.y += (int)(SPEED * speedMultiplier);
				bJumping = false;
			}
			else if (sprite->animation() != IDLE && sprite->animation() != WEAPON_IDLE)
				(numBullets > 0) ? sprite->changeAnimation(WEAPON_IDLE) : sprite->changeAnimation(IDLE);
		}
		// Si la flecha izquierda está pulsada
		else if (Game::instance().getKey(GLFW_KEY_LEFT))
		{
			facingRight = false;
			if (sprite->animation() == CLIMB && !map->collisionDown(pos, getSize(), 12.f)) {}	// Prohibido salir de la escalera a medias
			else {
				auto targetAnim = hasBullets() ? GUN_WALK_LEFT : WALK_LEFT;
				if (sprite->animation() != targetAnim && sprite->animation() != PUSH_LEFT) sprite->changeAnimation(targetAnim);
				incrLeft();
				// Si detecto colisión o se sale del mapa
				if (map->collisionMoveLeft(pos, getSize()) || pos.x < 0.f) incrRight();
			}
			// Si la animación actual no es moverse a la izquierda, cambio la animación a mover a la izquierda y le sumo desplazamiento
		}
		// Con la flecha derecha hago exactamente lo mismo
		else if (Game::instance().getKey(GLFW_KEY_RIGHT))
		{
			facingRight = true;
			if (sprite->animation() == CLIMB && !map->collisionDown(pos, getSize(), 12.f)) {}	// Prohibido salir de la escalera a medias
			else {
				auto targetAnim = hasBullets() ? GUN_WALK_RIGHT : WALK_RIGHT;
				if (sprite->animation() != targetAnim && sprite->animation() != PUSH_RIGHT) sprite->changeAnimation(targetAnim);
				incrRight();
				// Si detecto colisión o se sale del mapa
				if (map->collisionMoveRight(pos, getSize()) || pos.x > ((map->getMapSize().x - 1) * map->getTileSize()))
				{
					incrLeft();
				}			
			}
		}
		else inputDetected = false;
		// Si ninguna de las flechas está pulsada entonces dejo el personaje quieto mirando hacia el lado que corresponda

		if (!inputDetected) {
			if(sprite->animation() == CLIMB)
				sprite->setPaused(true);
			else if (sprite->animation() != IDLE && sprite->animation() != WEAPON_IDLE)
				(numBullets > 0) ? sprite->changeAnimation(WEAPON_IDLE) : sprite->changeAnimation(IDLE);
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
				if (jumpAngle > 90) {
					int jumpY = int(pos.y);
					if (map->collisionMoveDown(pos, getSize(), &jumpY, FALL_STEP) || onGround) {
						bJumping = false;
						pos.y = jumpY;
					}
				}
			}
		}
		else if (!map->collisionLadderUp(pos, getSize()) && !map->collisionLadderDown(pos, getSize()))
		{
			if (!onGround) pos.y += FALL_STEP;
			if(map->collisionMoveDown(pos, getSize(), &pos.y, FALL_STEP) || onGround)
			{
				onGround = true;

				if(Game::instance().getKey(GLFW_KEY_SPACE))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = pos.y;
				}
			}
			else {
				if (sprite->animation() != FALL)
					sprite->changeAnimation(FALL);
			}
		}
	}
	onGround = false;

	// Si el personaje se sale del mapa por abajo o por los lados, muere
	if (map->isOutOfMap(pos, size)) deathByMap = true;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}


void Player::setAnimation(const string& anim)
{
	if (anim == "IDLE")
		sprite->changeAnimation(IDLE);
	else if (anim == "WALK_LEFT")
		sprite->changeAnimation(WALK_LEFT);
	else if (anim == "WALK_RIGHT")
		sprite->changeAnimation(WALK_RIGHT);
	else if (anim == "DIE")
		sprite->changeAnimation(DIE);
	else if (anim == "FALL")
		sprite->changeAnimation(FALL);
	else if (anim == "CLIMB")
		sprite->changeAnimation(CLIMB);
	else if (anim == "OPEN_AND_ENTER")
		sprite->changeAnimation(OPEN_AND_ENTER);
	else if (anim == "ENTER")
		sprite->changeAnimation(ENTER);
	else if (anim == "TUNNEL_ENTER_BOTTOM")
		sprite->changeAnimation(TUNNEL_ENTER_BOTTOM);
	else if (anim == "TUNNEL_LEAVE_BOTTOM")
		sprite->changeAnimation(TUNNEL_LEAVE_BOTTOM);
	else if (anim == "TUNNEL_ENTER_TOP")
		sprite->changeAnimation(TUNNEL_ENTER_TOP);
	else if (anim == "TUNNEL_LEAVE_TOP")
		sprite->changeAnimation(TUNNEL_LEAVE_TOP);
	else if (anim == "PICK_ITEM")
		sprite->changeAnimation(PICK_ITEM);
	else if (anim == "PUSH_LEFT")
		sprite->changeAnimation(PUSH_LEFT);
	else if (anim == "PUSH_RIGHT")
		sprite->changeAnimation(PUSH_RIGHT);
	else if (anim == "GUN_WALK_LEFT")
		sprite->changeAnimation(GUN_WALK_LEFT);
	else if (anim == "GUN_WALK_RIGHT")
		sprite->changeAnimation(GUN_WALK_RIGHT);
	else if (anim == "WEAPON_IDLE")
		sprite->changeAnimation(WEAPON_IDLE);
	else if (anim == "START")
		sprite->changeAnimation(START);
	else
		cout << "Unknown animation: " << anim << endl;
}


string Player::getCurrentAnimationName() const {

	int currentAnimId = sprite->animation();

	switch (currentAnimId) {
	case IDLE:					return "IDLE";
	case WALK_LEFT:				return "WALK_LEFT";
	case WALK_RIGHT:			return "WALK_RIGHT";
	case DIE:					return "DIE";
	case FALL:					return "FALL";
	case CLIMB:					return "CLIMB";
	case OPEN_AND_ENTER:		return "OPEN_AND_ENTER";
	case ENTER:					return "ENTER";
	case TUNNEL_ENTER_BOTTOM:	return "TUNNEL_ENTER_BOTTOM";
	case TUNNEL_LEAVE_BOTTOM:	return "TUNNEL_LEAVE_BOTTOM";
	case TUNNEL_ENTER_TOP:		return "TUNNEL_ENTER_TOP";
	case TUNNEL_LEAVE_TOP:		return "TUNNEL_LEAVE_TOP";
	case PICK_ITEM:				return "PICK_ITEM";
	case PUSH_LEFT:				return "PUSH_LEFT";
	case PUSH_RIGHT:			return "PUSH_RIGHT";
	case GUN_WALK_LEFT:			return "GUN_WALK_LEFT";
	case GUN_WALK_RIGHT:		return "GUN_WALK_RIGHT";
	case WEAPON_IDLE:			return "WEAPON_IDLE";
	case START:					return "START";
	default:					return "UNKNOWN";
	}
}

void Player::incrRight()
{
	pos.x += (int)(SPEED * speedMultiplier);
}

void Player::incrLeft()
{
	pos.x -= (int)(SPEED * speedMultiplier);
}

void Player::incrUp(int px)
{
	pos.y -= (px + FALL_STEP); // Evitar gravedad
}

void Player::handlePush(int dir, bool pushSuccess) {
	switch (dir) 
	{
		case (1):
			if (pushSuccess && (sprite->animation() != WALK_RIGHT)) {
				sprite->changeAnimation(WALK_RIGHT);
			}
			else {
				if (sprite->animation() != PUSH_RIGHT)
					sprite->changeAnimation(PUSH_RIGHT);
			}
			break;
		case(-1):
			 if (pushSuccess && sprite->animation() != WALK_LEFT) {
				sprite->changeAnimation(WALK_LEFT);
			}
			else {
				if (sprite->animation() != PUSH_LEFT)
					sprite->changeAnimation(PUSH_LEFT);
			}
			 break;
		default:
			cout << "Invalid push direction: " << dir << endl;
			break;
	}
	
}

void Player::pickItem() {
	itemPickTimer = PICK_ITEM_TIMER;
	blockInput();
	if (sprite->animation() != PICK_ITEM) sprite->changeAnimation(PICK_ITEM);
}

void Player::activateSpeedBoost(float multiplier, float duration) {
	speedBoostTimer = duration;
	speedMultiplier = multiplier;
}

void Player::shoot()
{
	if (currentRoom == nullptr) {
		cout << "Player doesn't have a room assigned" << endl;
		return;
	}

	cout << "Shots fired! Remaining bullets: " << numBullets - 1 << endl;
	
	// Creación de la bala
	Bullet* bullet = new Bullet();
	bullet->init(glm::ivec2(tileMapDispl.x, tileMapDispl.y), *shaderProgram, cameraPtr);
	bullet->setDirection(facingRight);

	glm::ivec2 playerSize = this->getSize();
	glm::ivec2 bulletSize = bullet->getSize();
	
	glm::vec2  bulletPos;
	float gap = 6.f;

	if (facingRight) bulletPos.x = this->pos.x + playerSize.x + gap;
	else bulletPos.x = this->pos.x - bulletSize.x - gap;
	bulletPos.y = float(this->pos.y + 0.35f * playerSize.y);

	bullet->setPosition(bulletPos);
	bullet->setTileMap(map);
	currentRoom->addEntity(bullet);

	// Animación de disparo
	shootAnimTimer = 300;
	isShooting = true;
	blockInput();

	// [TODO] añadir sprite disparo 
	// sprite->changeAnimation(facingRight ? SHOOT_RIGHT : SHOOT_LEFT);
}

void Player::exitPipe(bool exitingUp) {
	if (exitingUp) {
		startAnimTimer = 750.0f;
		sprite->changeAnimation(START);
		blockInput();
	}
	// Sino ya tiene animación de caer
}

void Player::center() {
	pos.x = ((pos.x + map->getBlockSize() / 2) / map->getBlockSize()) * map->getBlockSize();
}