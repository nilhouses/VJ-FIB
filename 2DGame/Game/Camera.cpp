#include "Camera.h"
#include <iostream>
#include <algorithm>
using namespace std;

Camera::Camera(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight)
{
    position = glm::vec2(0.f, 0.f);
    target = glm::vec2(screenWidth / 2.f, screenHeight / 2.f);
}

// Actualiza la cámara para seguir al jugador u otro objetivo
void Camera::update(const glm::vec2& targetPos, const glm::vec2& mapSize)
{
    position.x = targetPos.x - target.x;
    position.y = targetPos.y - target.y + 32.0f;

    // Ajuste de límites (Clamping)
    position.x = std::max(0.f, std::min(position.x, mapSize.x - screenWidth));
 
    // Si el mapa es de 384 o menos, position.y será 0
    position.y = std::max(0.f, std::min(position.y, mapSize.y - screenHeight));

    position = glm::floor(position);
}


/* Se deben inicializar transitionStartPos, transitionEndPos y totalTransitionTime con los setters correspondientes */
void Camera::updateTransition(float time, const glm::vec2& mapSize)
{
	float timeElapsed = totalTransitionTime - time;

	// Calcular el target position usando interpolación lineal entre initPos y finPos
	glm::vec2 targetPos = transitionStartPos + (transitionEndPos - transitionStartPos) * (timeElapsed / totalTransitionTime);

	// Actualizar la posición de la cámara para seguir el target position
    update(targetPos, mapSize);
}


void Camera::printTransitionInfo() const
{
	cout << "Start Pos: (" << transitionStartPos.x << ", " << transitionStartPos.y << ")" << endl;
	cout << "End Pos: (" << transitionEndPos.x << ", " << transitionEndPos.y << ")" << endl;
	cout << "Total Transition Time: " << totalTransitionTime << endl;
}