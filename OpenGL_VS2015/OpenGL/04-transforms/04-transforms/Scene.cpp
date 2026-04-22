#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Scene.h"


Scene::Scene()
{
	quad = NULL;
}

Scene::~Scene()
{
	if(quad != NULL)
		delete quad;
}


void Scene::init()
{
	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, program);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render()
{
	glm::mat4 modelview;

	// We can now, using matrices, draw four quads at different screen locations
	// using a single Quad object.
	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	/*
		// Codi original
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();
	*/
		// [Nil] Codi modificat
	float oscillation = sin(currentTime / 500.f); // [-1.0, 1.0]

	// Escalat, (esquerra 1, dreta 0,25) 
	// 1) Passo l'oscil·lació de [-1, 1] a [0, 1]
	float t = (oscillation + 1.0f) / 2.0f;
	// 2) Interpolació lineal entre 1.0 i 0.25
	float s = glm::mix(1.0f, 0.25f, t);

	// Posicions base dels 4 quads
	glm::vec2 basePositions[4] = {
		glm::vec2(128.f, 48.f),
		glm::vec2(384.f, 48.f),
		glm::vec2(128.f, 304.f),
		glm::vec2(384.f, 304.f)
	};

	for (int i = 0; i < 4; i++) {
		modelview = glm::mat4(1.0f);
		// 4) Moure el quad horitzontalment segons deltaX
		modelview = glm::translate(modelview, glm::vec3(basePositions[i].x + 100*oscillation, basePositions[i].y, 0.f));
		// 3) Tornar a la posició inicial
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		// 2) Escalat
		modelview = glm::scale(modelview, glm::vec3(s, s, 1.0f));
		// 1) Posar el centre del quad a (0,0)
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		program.setUniformMatrix4f("modelview", modelview);
		quad->render();
	}

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if(!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
}

