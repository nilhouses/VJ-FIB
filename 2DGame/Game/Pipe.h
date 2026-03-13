#ifndef _PIPE_INCLUDE
#define _PIPE_INCLUDE

#include "Game.h"
#include "Entity.h"
#include "Camera.h"
#include "PipeSegment.h"
#include <vector>

class Pipe : public Entity
{
public:
    Pipe();
    ~Pipe();

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c,
        const vector<glm::ivec2>& tileSegments, int tileSize = 32);

    void update(int deltaTime) override;
    void render()              override;

    glm::vec4 getEndBoundingBox(int end) const;
    glm::vec4 getBoundingBox() const override;
    glm::vec2 getEndDirection(int end) const;

    void startTransit(int entryEnd);
    bool isTransitComplete() const { return transitComplete; }
    glm::vec2 getExitPosition(int playerHeight = 64) const;
    bool getEntryKey(int end) const;

private:
    glm::vec2 endTexCoords(const glm::ivec2& dirToAdjacent) const;
    glm::vec2 inferTexCoords(const vector<glm::ivec2>& tiles, int i) const;
    glm::vec2 Pipe::busyTexCoords(const glm::vec2& idleTexCoords) const;
    vector<PipeSegment> segments;

    int   entryEnd = 0;
    bool  someoneInside = false;
    bool  transitComplete = false;
    float transitProgress = 0.f;
    float transitDuration = 800.f;
    int   activeSegment = 0;

    ShaderProgram* shaderProg = nullptr;
    Camera* cam = nullptr;
};

#endif // _PIPE_INCLUDE