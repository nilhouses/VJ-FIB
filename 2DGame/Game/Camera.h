#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int screenWidth, int screenHeight);

    // Actualiza la cámara para seguir al jugador u otro objetivo
    void update(const glm::vec2& targetPos, const glm::vec2& mapSize);
	void updateTransition(float time, const glm::vec2& mapSize);

	void setTotalTimer(float totalTime) { totalTransitionTime = totalTime; }
	void setStartPos(const glm::vec2& startPos) { transitionStartPos = startPos; }
	void setEndPos(const glm::vec2& endPos) { transitionEndPos = endPos; }

    glm::vec2 getPosition() const { return position; }

    // Devuelve un offset para aplicar en modelview (para shaders)
    glm::vec2 getOffset() const { return -position; }

    void printTransitionInfo() const;
    bool isVisible(const glm::vec2& worldPos) const {
        return worldPos.x >= position.x && worldPos.x <= position.x + screenWidth &&
            worldPos.y >= position.y && worldPos.y <= position.y + screenHeight;
    }

private:
    glm::vec2 position = glm::vec2(0.f,0.f);            // Posición de la cámara en el mundo
    glm::vec2 target = glm::vec2(0.f, 0.f);             // Punto de la pantalla donde queremos centrar al jugador
    int screenWidth = 640;
    int screenHeight = 480;

	float totalTransitionTime = 0.f;                    // Duración total de la transición (para updateTransition)
	glm::vec2 transitionStartPos = glm::vec2(0.f, 0.f); // Posición inicial de la transición
	glm::vec2 transitionEndPos = glm::vec2(0.f, 0.f);   // Posición final de la transición
};

#endif
