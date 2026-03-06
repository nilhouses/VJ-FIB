#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4	// Velocidad del salto del jugador, cuanto más alto más lento será el salto
#define JUMP_HEIGHT 96		// Altura máxima del salto del jugador
#define FALL_STEP 4			// Velocidad de caída del jugador
#define SPEED 2 			// Velocidad de movimiento del jugador
#define PICK_ITEM_TIMER 500 // Duración de la animación de recoger un item (ms)

// Definimos tipos de animaciones para el jugador
enum PlayerAnims
{
	IDLE, WALK_LEFT, WALK_RIGHT, DIE, FALL, CLIMB, OPEN_AND_ENTER, ENTER, ENTER_TUNEL, EXIT_TUNEL, PICK_ITEM, PUSH_LEFT, PUSH_RIGHT, GUN_WALK_LEFT, GUN_WALK_RIGHT, NUM_ANIMS
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
	Entity::init(tileMapPos, shaderProgram, "images/playerSheet.png", glm::ivec2(32, 64), glm::vec2(1.f/35.f, 1.f/7.f), c);

	// Inicializar los atributos del Player
	bJumping = false;
	jumpAngle = 0;
	startY = 0;
	blockedInput = false;
	itemPickTimer = 0;
	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(IDLE, 4);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 35.f, 0.f));

	sprite->setAnimationSpeed(WALK_LEFT, 8);
	sprite->addKeyframe(WALK_LEFT, glm::vec2(2.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(3.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(4.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_LEFT, glm::vec2(5.f / 35.f, 0.f));

	sprite->setAnimationSpeed(WALK_RIGHT, 8);
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(6.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(7.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(8.f / 35.f, 0.f));
	sprite->addKeyframe(WALK_RIGHT, glm::vec2(9.f / 35.f, 0.f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(10.f / 35.f, 0.f));
	sprite->addKeyframe(DIE, glm::vec2(11.f / 35.f, 0.f));
	sprite->addKeyframe(DIE, glm::vec2(12.f / 35.f, 0.f));
	sprite->addKeyframe(DIE, glm::vec2(13.f / 35.f, 0.f));

	sprite->setAnimationSpeed(FALL, 20);
	sprite->addKeyframe(FALL, glm::vec2(15.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(FALL, glm::vec2(16.f / 35.f, 1.f / 7.f));

	sprite->setAnimationSpeed(CLIMB, 12);
	sprite->addKeyframe(CLIMB, glm::vec2(14.f / 35.f, 0.f));
	sprite->addKeyframe(CLIMB, glm::vec2(15.f / 35.f, 0.f));
	sprite->addKeyframe(CLIMB, glm::vec2(16.f / 35.f, 0.f));
	sprite->addKeyframe(CLIMB, glm::vec2(17.f / 35.f, 0.f));

	sprite->setAnimationSpeed(OPEN_AND_ENTER, 8);
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(18.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(19.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(20.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(21.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(22.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(23.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(24.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(25.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(26.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(27.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(28.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(29.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(30.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(31.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(32.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(33.f / 35.f, 0.f));
	sprite->addKeyframe(OPEN_AND_ENTER, glm::vec2(34.f / 35.f, 0.f));

	sprite->setAnimationSpeed(ENTER, 8);
	sprite->addKeyframe(ENTER, glm::vec2(20.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(21.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(22.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(23.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(24.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(25.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(26.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(27.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(ENTER, glm::vec2(28.f / 35.f, 1.f / 7.f));

	sprite->setAnimationSpeed(ENTER_TUNEL, 8);
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(10.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(11.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(12.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(13.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(14.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(15.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(16.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(17.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(18.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(19.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(20.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(21.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(22.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(23.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(24.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(ENTER_TUNEL, glm::vec2(25.f / 35.f, 2.f / 7.f));

	sprite->setAnimationSpeed(EXIT_TUNEL, 8);
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(26.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(27.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(28.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(29.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(30.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(31.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(32.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(EXIT_TUNEL, glm::vec2(33.f / 35.f, 2.f / 7.f));

	sprite->setAnimationSpeed(PICK_ITEM, 10);
	sprite->addKeyframe(PICK_ITEM, glm::vec2(10.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(11.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(12.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(13.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(PICK_ITEM, glm::vec2(14.f / 35.f, 1.f / 7.f));

	sprite->setAnimationSpeed(PUSH_LEFT, 8);
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(2.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(3.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(4.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_LEFT, glm::vec2(5.f / 35.f, 2.f / 7.f));

	sprite->setAnimationSpeed(PUSH_RIGHT, 8);
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(6.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(7.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(8.f / 35.f, 2.f / 7.f));
	sprite->addKeyframe(PUSH_RIGHT, glm::vec2(9.f / 35.f, 2.f / 7.f));

	sprite->setAnimationSpeed(GUN_WALK_LEFT, 8);
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(2.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(3.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(4.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_LEFT, glm::vec2(5.f / 35.f, 1.f / 7.f));

	sprite->setAnimationSpeed(GUN_WALK_RIGHT, 8);
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(6.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(7.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(8.f / 35.f, 1.f / 7.f));
	sprite->addKeyframe(GUN_WALK_RIGHT, glm::vec2(9.f / 35.f, 1.f / 7.f));

	sprite->changeAnimation(IDLE);
}


// Esta función solo calcula la posición actual y carga en el sprite la animación correspondiente en cada caso
void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	int tempY = int(pos.y);
	bool inputDetected = true;

	if (sprite->animation() == PICK_ITEM) {
		itemPickTimer -= deltaTime;
		if (itemPickTimer <= 0) {
			unblockInput();
			sprite->changeAnimation(IDLE);
		}
	}

	if (!blockedInput) {

		// Con la flecha hacia arriba el personaje subirá si existe una escalera en esa posición
		if (Game::instance().getKey(GLFW_KEY_UP)) {
			if (map->collisionLadderUp(pos, getSize())) {
				if (sprite->animation() != CLIMB)
					sprite->changeAnimation(CLIMB);
				
				sprite->setPaused(false);
				pos.y -= SPEED;
				bJumping = false;
			}
			else if (sprite->animation() != IDLE) sprite->changeAnimation(IDLE);
		}
		// Con la flecha hacia abajo el personaje bajará si existe una escalera en esa posición
		else if (Game::instance().getKey(GLFW_KEY_DOWN)) {
			if (map->collisionLadderDown(pos, getSize())) {
				if (sprite->animation() != CLIMB)
					sprite->changeAnimation(CLIMB);

				sprite->setPaused(false);
				pos.y += SPEED;
				bJumping = false;
			}
			else if (sprite->animation() != IDLE) sprite->changeAnimation(IDLE);
		}
		// Si la flecha izquierda está pulsada
		else if (Game::instance().getKey(GLFW_KEY_LEFT))
		{
			int jumpY = int(pos.y);
			if (sprite->animation() == CLIMB && !map->collisionMoveDown(pos, getSize(), &jumpY, FALL_STEP)) {}	// Prohibido salir de la escalera a medias
			else {
				if (sprite->animation() != WALK_LEFT && sprite->animation() != PUSH_LEFT)
					sprite->changeAnimation(WALK_LEFT);
				incrLeft();
				// Si detecto colisión o se sale del mapa
				if (map->collisionMoveLeft(pos, getSize()) || pos.x < 0.f)
				{
					incrRight();
				}
			}
			// Si la animación actual no es moverse a la izquierda, cambio la animación a mover a la izquierda y le sumo desplazamiento
		}
		// Con la flecha derecha hago exactamente lo mismo
		else if (Game::instance().getKey(GLFW_KEY_RIGHT))
		{
			int jumpY = int(pos.y);
			if (sprite->animation() == CLIMB && !map->collisionMoveDown(pos, getSize(), &jumpY, FALL_STEP)) {}	// Prohibido salir de la escalera a medias
			else {
				if (sprite->animation() != WALK_RIGHT && sprite->animation() != PUSH_RIGHT)
					sprite->changeAnimation(WALK_RIGHT);
				// Si la animación actual no es moverse a la derecha, cambio la animación a mover a la izquierda y le sumo desplazamiento
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
			else if (sprite->animation() != IDLE)
				sprite->changeAnimation(IDLE);
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
						pos.y = float(jumpY);
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
	else if (anim == "ENTER_TUNEL")
		sprite->changeAnimation(ENTER_TUNEL);
	else if (anim == "EXIT_TUNEL")
		sprite->changeAnimation(EXIT_TUNEL);
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
}


string Player::getCurrentAnimationName() const {

	int currentAnimId = sprite->animation();

	switch (currentAnimId) {
	case IDLE:				return "IDLE";
	case WALK_LEFT:			return "WALK_LEFT";
	case WALK_RIGHT:		return "WALK_RIGHT";
	case DIE:				return "DIE";
	case FALL:				return "FALL";
	case CLIMB:				return "CLIMB";
	case OPEN_AND_ENTER:    return "OPEN_AND_ENTER";
	case ENTER:				return "ENTER";
	case ENTER_TUNEL:		return "ENTER_TUNEL";
	case EXIT_TUNEL:        return "EXIT_TUNEL";
	case PICK_ITEM:         return "PICK_ITEM";
	case PUSH_LEFT:         return "PUSH_LEFT";
	case PUSH_RIGHT:        return "PUSH_RIGHT";
	case GUN_WALK_LEFT:     return "GUN_WALK_LEFT";
	case GUN_WALK_RIGHT:    return "GUN_WALK_RIGHT";
	default:				return "UNKNOWN";
	}
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