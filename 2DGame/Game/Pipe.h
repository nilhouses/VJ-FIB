#ifndef _PIPE_INCLUDE
#define _PIPE_INCLUDE

#include "Game.h"
#include "Entity.h"
#include "Camera.h"
#include "PipeSegment.h"
#include "SoundManager.h"
#include "EnterArrow.h"
#include <vector>

class Pipe : public Entity
{
public:
    Pipe();
    ~Pipe();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, const vector<glm::ivec2>& tileSegments, int tileSize = 32, int sr = 0);

    void update(int deltaTime) override;
    void render()              override;
    
    void activateArrow(int end);
    void setRoomInt(int r) override;
    void setPosition(const glm::vec2& pos) override;

    glm::vec4 getEndBoundingBox(int end) const;
    glm::vec4 getBoundingBox() const override;
    glm::vec2 getEndDirection(int end) const;

    bool isOccupied() const { return someoneInside; }
    void startTransit(int entryEnd, bool withSound);
    bool isTransitComplete() const { return transitComplete; }
    bool isExitingUp() const {
        int exitEnd = (entryEnd == 0) ? 1 : 0;
        return getEndDirection(exitEnd).y > 0;
    }
    bool getEntryKey(int end) const;
    glm::vec2 getExitPosition(int playerHeight) const;              // usa entryEnd
    glm::vec2 getExitPosition(int playerHeight, int fromEnd) const; // usa fromEnd

    float calculateTotalTime() { return transitDuration; }

private:
    glm::vec2 endTexCoords(const glm::ivec2& dirToAdjacent) const;
    glm::vec2 inferTexCoords(const vector<glm::ivec2>& tiles, int i) const;
    glm::vec2 busyTexCoords(const glm::vec2& idleTexCoords) const;
    vector<PipeSegment> segments = vector<PipeSegment>();

    int   entryEnd = 0;
    bool  someoneInside = false;
    bool  transitComplete = false;
    float transitProgress = 0.f;
    float transitDuration = 0.f; // se calcula en init()
    int activeSegment = 0;
    bool soundEnabled = false;
    int tileSize = 32;

    ShaderProgram* shaderProg = nullptr;
    Camera* cam = nullptr;
    EnterArrow* arrows[2] = { nullptr, nullptr };
};

#endif // _PIPE_INCLUDE