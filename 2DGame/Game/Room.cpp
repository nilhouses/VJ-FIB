#include "Room.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>


// ---------------------- CONSTRUCTORS ----------------------

Room::Room(ShaderProgram& shaderProgram)
	: texProgram(shaderProgram)
{
    map = nullptr;
}

Room::~Room()
{
    if (map != NULL)
        delete map;
	for (Entity* entity : entities) {
        if (entity != nullptr)
            delete entity;
    }
	entities.clear();
    for (Entity* enemy : enemies) {
        if (enemy != nullptr)
            delete enemy;
    }
    enemies.clear();
    for (Asset* asset : assets) {
        if (asset != nullptr)
            delete asset;
	}
	assets.clear();
}

// ---------------------- INIT ----------------------

void Room::init()
{
	transitioning = false;
}


// ---------------------- UPDATE -----------------------


void Room::update(int deltaTime)
{
    // Solo actualizo la animación del jugador durante la transición y se mantiene bloqueada su entrada
    
    // Actualizar entidades
    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (!transitioning || entities[i]->getType() == Type::ENTER) {
            if (entities[i]->isActive())
                entities[i]->update(deltaTime);
        }
    }
}


// ---------------------- RENDER ----------------------

void Room::render(Camera* camera, glm::mat4& projection)
{
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(camera->getOffset(), 0.0f)
    );
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    /*
        Orden de renderizado:
        1. Fondo
        2. tileMapBase
        3. Assets
        4. tileMapFront
        5. Entidades
        6. HUD
    */
	background->render();

    map->renderBase();

    for (Asset* asset : assets)
    {
        asset->render();
    }

    // Los assets actualizan la modelview así que devolvemos la modelview original para renderizar el tilemap front
    texProgram.setUniformMatrix4f("modelview", modelview);
    map->renderFront();
    map->renderFront2();

    for (size_t i = 0; i < entities.size(); ++i)
    {
        if (entities[i]->isActive())
            entities[i]->render();
    }
}

void Room::setBackground(Asset* b)
{
    background = b;
	background->setParrallax(5.0f);
}