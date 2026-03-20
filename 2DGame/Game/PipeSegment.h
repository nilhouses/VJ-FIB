#ifndef _PIPE_SEGMENT_INCLUDE
#define _PIPE_SEGMENT_INCLUDE

#include "Sprite.h"
#include <glm/glm.hpp>

enum PipeSegmentAnim { PIPE_IDLE, PIPE_BUSY, PIPE_NUM_ANIMS };

class PipeSegment
{

public:
    void init(Sprite* s, const glm::vec2& idleTexCoords, const glm::vec2& busyTexCoords, const glm::vec2& tilePos);
    void setActive(bool active);
    void update(int deltaTime);
    void render();

    glm::vec2 getTilePos() const { return tilePos; }
    Sprite* getSprite() const { return sprite; }
    int getKeyFramesPerSecond() { return busyKeyFramesPerSecond;}
private:
    Sprite* sprite = nullptr;
    glm::vec2 tilePos;
    glm::vec2 idleTexCoords;
	int busyKeyFramesPerSecond = 16;
};

#endif