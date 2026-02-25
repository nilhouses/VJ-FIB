#include "Camera.h"
#include <algorithm>

Camera::Camera(int screenWidth, int screenHeight, int hudHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight), hudHeight(hudHeight)
{
    position = glm::vec2(0.f, 0.f);
    target = glm::vec2(screenWidth / 2.f, (screenHeight - hudHeight) / 2.f);
}

// Actualiza la cámara para seguir al jugador u otro objetivo
void Camera::update(const glm::vec2& targetPos, const glm::vec2& mapSize)
{
    position.x = targetPos.x - target.x;
    position.y = targetPos.y - target.y;

    // Clamp para que la cámara no salga del mapa
    position.x = std::max(0.f, std::min(position.x, mapSize.x - screenWidth));
    position.y = std::max(0.f, std::min(position.y, mapSize.y - (screenHeight - hudHeight)));

    // Para evitar subpixel rendering
    position = glm::floor(position);
}
