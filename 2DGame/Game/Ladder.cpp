#include "Ladder.h"


enum LadderAnims
{
	STAND, NUM_ANIMS
};


Ladder::Ladder()
{

}

Ladder::~Ladder()
{
	if (sprite != NULL)
		delete sprite;
}

void Ladder::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c)
{
	// Inicializar los atributos de la Entity
	Entity::init(tileMapPos, shaderProgram, "images/tilesheet.png", glm::ivec2(32, 32), glm::vec2(0.25f, 0.25f), c);

	// Inicializar los atributos de Ladder

	// Configuración de animaciones
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0.25f, 0.25f));

	sprite->changeAnimation(STAND);
}

void Ladder::update(int deltaTime)
{
	
}

bool Ladder::contains(const glm::vec2& playerPos) const
{
	// Comprobar si la posición del jugador está dentro de la escalera
	return playerPos.x >= position.x && playerPos.x <= position.x + 32 &&
		   playerPos.y >= position.y && playerPos.y <= position.y + 32;
}
