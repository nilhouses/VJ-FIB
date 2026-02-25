#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int screenWidth, int screenHeight, int hudHeight = 0);

    // Actualiza la cámara para seguir al jugador u otro objetivo
    void update(const glm::vec2& targetPos, const glm::vec2& mapSize);

    glm::vec2 getPosition() const { return position; }

    // Devuelve un offset para aplicar en modelview (para shaders)
    glm::vec2 getOffset() const { return -position + glm::vec2(0.f, hudHeight); }

private:
    glm::vec2 position;  // Posición de la cámara en el mundo
    glm::vec2 target;    // Punto de la pantalla donde queremos centrar al jugador
    int screenWidth;
    int screenHeight;
    int hudHeight;
};

#endif
