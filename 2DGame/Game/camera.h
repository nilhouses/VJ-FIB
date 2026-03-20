#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int screenWidth, int screenHeight, int hudHeight = 0);

    // Actualiza la cámara para seguir al jugador u otro objetivo
    void update(const glm::vec2& targetPos, const glm::vec2& mapSize);
	void updateTransition(float time, const glm::vec2& mapSize);

	void setTotalTimer(float totalTime) { totalTransitionTime = totalTime; }
	void setStartPos(const glm::vec2& startPos) { transitionStartPos = startPos; }
	void setEndPos(const glm::vec2& endPos) { transitionEndPos = endPos; }

    glm::vec2 getPosition() const { return position; }

    // Devuelve un offset para aplicar en modelview (para shaders)
    glm::vec2 getOffset() const { return -position + glm::vec2(0.f, hudHeight); }

    void printTransitionInfo() const;

private:
    glm::vec2 position;  // Posición de la cámara en el mundo
    glm::vec2 target;    // Punto de la pantalla donde queremos centrar al jugador
    int screenWidth;
    int screenHeight;
    int hudHeight;

	float totalTransitionTime; // Duración total de la transición (para updateTransition)
	glm::vec2 transitionStartPos; // Posición inicial de la transición
	glm::vec2 transitionEndPos;   // Posición final de la transición
};

#endif
