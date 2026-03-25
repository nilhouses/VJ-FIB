#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, Camera* c)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program, c);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, Camera* c)
{
	// Margen de seguridad para evitar texture bleeding
	float eps = 0.0005f;

	// Definimos las coordenadas de textura encogidas
	float uStart = 0.f + eps;
	float vStart = 0.f + eps;
	float uEnd = sizeInSpritesheet.x - eps;
	float vEnd = sizeInSpritesheet.y - eps;

	float vertices[24] = {
		0.f, 0.f, uStart, vStart,
		quadSize.x, 0.f, uEnd, vStart,
		quadSize.x, quadSize.y, uEnd, vEnd,
		0.f, 0.f, uStart, vStart,
		quadSize.x, quadSize.y, uEnd, vEnd,
		0.f, quadSize.y, uStart, vEnd
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
	camera = c;
}

Sprite::~Sprite()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Sprite::update(int deltaTime)
{
	if((currentAnimation >= 0) && !isPaused)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::vec2 offsetCam = camera->getOffset();

	float finalX = std::floor(position.x + offsetCam.x / fParallax);
	float finalY = std::floor(position.y + offsetCam.y / fParallax);

	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(finalX, finalY, 0.f));

	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);

	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);

	// Dejo el modelview como estaba por si se quiere renderizar algo más después del sprite
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + offsetCam.x, position.y + offsetCam.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
}

void Sprite::render(const glm::mat4& proj) const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("projection", proj);
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{

	if(animId < int(animations.size()))
	{
		setPaused(false);
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}



