#include <iostream>
#include "Scene.h"
#include "Circle.h"
#include "Game.h" // Per obtenir la RA del viewport

Scene::Scene()
{
	for(int i = 0; i < 4; i++)
		quads[i] = NULL;
	circle = NULL;
}

Scene::~Scene()
{
	for(int i = 0; i < 4; i++)
		if(quads[i] != NULL)
			delete quads[i];
	delete circle;
}


void Scene::init()
{
	initShaders();

	float RA = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
	float invRA = 1 / RA;
	for (int i = 0; i < 4; i++) {
		// [Nil] Li sumo a la coordenada x l'offset (mida antiga - mida nova)/2
		quads[i] = Quad::createQuad(-0.75f + (i % 2) + (0.5f - 0.5f * invRA) / 2.0f, 
			-0.75f + (i / 2),
			0.5f * invRA,
			0.5f,
			program);
		// Ho he adaptat a la RA. Codi anterior:
		// quads[i] = Quad::createQuad(-0.75f + (i % 2), -0.75f + (i / 2), 0.5f, 0.5f, program
	}
	// Afegeixo, també, el cercle, classe creada
	circle = Circle::createCircle(0.0f, 0.0f, 0.25f, program);
}


void Scene::update(int deltaTime)
{
}

void Scene::render()
{
	program.use();
	for(int i = 0; i < 4; i++)
		quads[i]->render();
	circle->render();
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

