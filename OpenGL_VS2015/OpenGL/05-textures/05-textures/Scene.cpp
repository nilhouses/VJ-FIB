#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	quad = NULL;
}

Scene::~Scene()
{
	if(quad != NULL)
		delete quad;
	for(int i=0; i<3; i++)
		if(texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
	//Les texCoords van de la part superior esquerra a la part inferior dreta de la textura, per tant (0,0) es la part superior esquerra i (1,1) la part inferior dreta que es vol pintar
	/*
	(0,0)


					(1,1)
	
	*/
	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, simpleProgram);
	// Mario
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram); 
	//texCoords[0] = glm::vec2(0.5f, 0.5f); texCoords[1] = glm::vec2(1.f, 1.f); // Estrella
	// Bolet
	texCoords[0] = glm::vec2(0.0f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);  
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram); 
	// Terra
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(2*float(CAMERA_WIDTH/128.0f), 2.f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram); 
	// Load textures
	texs[0].loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/brick.png", TEXTURE_PIXEL_FORMAT_RGB);
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

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	//simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f); // Blau original
	simpleProgram.setUniform4f("color", 0.5f, 0.7f, 1.0f, 1.0f); // Blau cel

	// 1) Cel
	glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
	// Alternativa més complexa
	/* float escalaX = float(CAMERA_WIDTH) / 128.f;
	float escalaY = float(CAMERA_HEIGHT) / 128.f;
	modelview = glm::mat4(1.0f); // (0,0)
	modelview = glm::scale(modelview, glm::vec3(escalaX, escalaY, 1.0f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	quad->render();*/

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	

	// 2) Terra
	texProgram.use();
	float yTerra = float(CAMERA_HEIGHT) - 128.f; 
	modelview = glm::translate(modelview, glm::vec3(0.f, yTerra, 0.f));
	float escalaTilesX = float(CAMERA_WIDTH) / 128.f;
	modelview = glm::scale(modelview, glm::vec3(escalaTilesX, 1.0f, 1.0f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[1]);

	// 3) Bolet
	float yBase = yTerra - 128.f;
	float yAjustada = yBase + 33.f; // Error de la textura
	// El bolet es mou d'esquerra a dreta
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.0f + 128*sin(float(currentTime/500.f)), yAjustada, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	/*
	
	// Codi original

	// 1) Blue Sky
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	quad->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// 2) Mario
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);

	// 3) Star
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	// 4) Rock
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(384.f, 304.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[1]);
	*/
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
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if(!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

