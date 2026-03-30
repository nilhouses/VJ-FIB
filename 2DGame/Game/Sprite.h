#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"
#include "Camera.h"
#include <iostream>


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, Camera* c);
	~Sprite();

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, Camera* c);

	void update(int deltaTime);
	void render() const;
	void render(const glm::mat4& proj) const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	int animation() const;
	
	void setPosition(const glm::vec2 &pos);
	void setPaused(bool paused) { isPaused = paused; }
	
	void setParallax(float f) { fParallax = f; }
	bool isLastKeyframe() const { return currentKeyframe == (int)animations[currentAnimation].keyframeDispl.size() - 1; }

private:
	Texture *texture = nullptr;
	ShaderProgram *shaderProgram = nullptr;
	GLuint vao = GLuint();
	GLuint vbo = GLuint();
	GLint posLocation = GLint(), texCoordLocation = GLint();
	glm::vec2 position = glm::vec2(0.f,0.f);
	int currentAnimation = 0, currentKeyframe = 0;
	float timeAnimation = 0.f;
	glm::vec2 texCoordDispl = glm::vec2(0.f,0.f);
	std::vector<AnimKeyframes> animations = std::vector<AnimKeyframes>();
	Camera* camera = nullptr;
	bool isPaused = false;
	float fParallax = 1.f;
};


#endif // _SPRITE_INCLUDE

