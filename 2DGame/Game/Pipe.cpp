#include <GL/glew.h>
#include "Pipe.h"
#include <algorithm>
#include <iostream>

// Coordenadas de textura de cada tile del spritesheet pipe.png, (En función de donde tiene los agujeros)
#define ENTRY_DOWN         glm::vec2(0.00f, 0.00f)
#define ENTRY_UP           glm::vec2(0.25f, 0.00f)
#define VERTICAL           glm::vec2(0.50f, 0.00f)
#define HORIZONTAL         glm::vec2(0.75f, 0.00f)
#define FAT_VERTICAL       glm::vec2(0.50f, 0.55f)
#define FAT_HORIZONTAL     glm::vec2(0.75f, 0.50f)
#define TOP_RIGHT          glm::vec2(0.00f, 0.50f)
#define TOP_LEFT           glm::vec2(0.75f, 0.25f)
#define BOTTOM_RIGHT       glm::vec2(0.00f, 0.25f)
#define BOTTOM_LEFT        glm::vec2(0.25f, 0.25f)

Pipe::Pipe() : Entity(Type::PIPE) {}

Pipe::~Pipe()
{
    // El segmento 0 lo destruye Entity::~Entity() via sprite
    // Los demás (1..n) esta clase
    for (int i = 1; i < (int)segments.size(); ++i)
    {
        Sprite* s = segments[i].getSprite();
        if (s) delete s;
    }
}

// ------------------------------------------------------- Lògica para saber qué textura tiene cada segmento -------------------------------------------------------

glm::vec2 Pipe::endTexCoords(const glm::ivec2& dirToAdjacent) const
{
    if (dirToAdjacent == glm::ivec2(0, +1)) return ENTRY_DOWN;
    if (dirToAdjacent == glm::ivec2(0, -1)) return ENTRY_UP;
    return HORIZONTAL;
}

glm::vec2 Pipe::inferTexCoords(const vector<glm::ivec2>& tiles, int i) const
{
    int n = (int)tiles.size();
    if (i == 0)     return endTexCoords(tiles[1] - tiles[0]);
    if (i == n - 1) return endTexCoords(tiles[n - 2] - tiles[n - 1]);

    glm::ivec2 dirIn = tiles[i] - tiles[i - 1];
    glm::ivec2 dirOut = tiles[i + 1] - tiles[i];

    if (dirIn == dirOut) return (dirIn.x != 0) ? HORIZONTAL : VERTICAL;

    bool openTop = (dirIn == glm::ivec2(0, +1)) || (dirOut == glm::ivec2(0, -1));
    bool openBottom = (dirIn == glm::ivec2(0, -1)) || (dirOut == glm::ivec2(0, +1));
    bool openRight = (dirIn == glm::ivec2(-1, 0)) || (dirOut == glm::ivec2(+1, 0));
    bool openLeft = (dirIn == glm::ivec2(+1, 0)) || (dirOut == glm::ivec2(-1, 0));

    if (openTop && openRight) return TOP_RIGHT;
    if (openTop && openLeft)  return TOP_LEFT;
    if (openBottom && openRight) return BOTTOM_RIGHT;
    if (openBottom && openLeft)  return BOTTOM_LEFT;

    return VERTICAL;
}

glm::vec2 Pipe::busyTexCoords(const glm::vec2& idleTexCoords) const
{
    if (idleTexCoords == VERTICAL)   return FAT_VERTICAL;
    if (idleTexCoords == HORIZONTAL) return FAT_HORIZONTAL;
    return idleTexCoords; // Los demás segmentos no cambian
}


// ------------------------------------------------------- Init -------------------------------------------------------

void Pipe::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, const vector<glm::ivec2>& tileSegments, int tileSize)
{
    shaderProg = &shaderProgram;
    cam = c;

    Entity::init(tileMapPos, shaderProgram, "images/pipe.png",
        glm::ivec2(tileSize, tileSize), glm::vec2(0.25f, 0.25f), c);

    for (int i = 0; i < (int)tileSegments.size(); ++i)
    {
        glm::vec2 tilePos = glm::vec2(tileSegments[i].x * tileSize, tileSegments[i].y * tileSize);
        Sprite* s;
        if (i == 0) s = sprite;
        else s = Sprite::createSprite(glm::ivec2(tileSize, tileSize), glm::vec2(0.25f, 0.25f), &spritesheet, shaderProg, cam);
        s->setPosition(glm::vec2(tileMapPos.x + tilePos.x, tileMapPos.y + tilePos.y));


        glm::vec2 texCoords = inferTexCoords(tileSegments, i);
        glm::vec2 busyCoords = busyTexCoords(texCoords);
        
        PipeSegment seg;
        seg.init(s, texCoords, busyCoords, tilePos);
        segments.push_back(seg);
    }

    pos = glm::vec2(tileSegments[0].x * tileSize, tileSegments[0].y * tileSize);
    size = glm::ivec2(tileSize, tileSize);
}

// ------------------------------------------------------- Update -------------------------------------------------------

void Pipe::update(int deltaTime)
{
    for (auto& seg : segments) seg.update(deltaTime);

    if (!someoneInside) return;

    transitProgress += deltaTime / transitDuration;
    if (transitProgress > 1.f) transitProgress = 1.f;

    int total = (int)segments.size();
    int newActive;
    if (entryEnd == 0) newActive = max(0, min((int)(transitProgress * total), total - 1));
    else newActive = max(0, min((total - 1) - (int)(transitProgress * total), total - 1));

    if (newActive != activeSegment)
    {
        segments[activeSegment].setActive(false);
        activeSegment = newActive;
        segments[activeSegment].setActive(true);
    }

    if (transitProgress >= 1.f)
    {
        segments[activeSegment].setActive(false);
        someoneInside = false;
        transitComplete = true;
    }
}

void Pipe::render()
{
    if (!active) return;
    for (auto& seg : segments) seg.render();
}

// ------------------------------------------------------- Tráfico -------------------------------------------------------

void Pipe::startTransit(int end)
{
    entryEnd = end;
    someoneInside = true;
    transitComplete = false;
    transitProgress = 0.f;
    activeSegment = (end == 0) ? 0 : (int)segments.size() - 1;
    segments[activeSegment].setActive(true);
}

glm::vec2 Pipe::getExitPosition(int playerHeight) const
{
    int exitEnd = (entryEnd == 0) ? 1 : 0;
    int idx = (exitEnd == 0) ? 0 : (int)segments.size() - 1;
    glm::vec2 tp = segments[idx].getTilePos();
    glm::vec2 dir = getEndDirection(exitEnd);
    glm::vec2 off = -dir * float(size.x);
    glm::vec2 base = glm::vec2(tileMapDispl.x + tp.x + off.x,
        tileMapDispl.y + tp.y + off.y);

    base.y = base.y + size.y - playerHeight;

    return base;
}

// ------------------------------------------------------- BoundingBox -------------------------------------------------------

glm::vec4 Pipe::getEndBoundingBox(int end) const
{
    int idx = (end == 0) ? 0 : (int)segments.size() - 1;
    glm::vec2 tp = segments[idx].getTilePos();
    glm::vec2 dir = getEndDirection(end);
    glm::vec2 off = -dir * float(size.x); // Justo encima o abajo del tubo

    return glm::vec4(tileMapDispl.x + tp.x + off.x, tileMapDispl.y + tp.y + off.y, size.x, size.y);
}

bool Pipe::getEntryKey(int end) const
{
    int idx = (end == 0) ? 0 : (int)segments.size() - 1;
    int nextIdx = (end == 0) ? 1 : (int)segments.size() - 2;

    // La direcció cap a l'interior del tub
    glm::vec2 dir = segments[nextIdx].getTilePos() - segments[idx].getTilePos();

    if (dir.y > 0) return Game::instance().getKey(GLFW_KEY_DOWN);
    if (dir.y < 0) return Game::instance().getKey(GLFW_KEY_UP);
    return false;
}

// Override
glm::vec4 Pipe::getBoundingBox() const // Cubre ambos extremos de la tubería
{
    glm::vec4 b0 = getEndBoundingBox(0);
    glm::vec4 b1 = getEndBoundingBox(1);

    float minX = min(b0.x, b1.x);
    float minY = min(b0.y, b1.y);
    float maxX = max(b0.x + b0.z, b1.x + b1.z);
    float maxY = max(b0.y + b0.w, b1.y + b1.w);

    return glm::vec4(minX, minY, maxX - minX, maxY - minY);
}

glm::vec2 Pipe::getEndDirection(int end) const
{
    int idx = (end == 0) ? 0 : (int)segments.size() - 1;
    int nextIdx = (end == 0) ? 1 : (int)segments.size() - 2;
    glm::vec2 dir = segments[nextIdx].getTilePos() - segments[idx].getTilePos();
    // Normalizar
    if (dir.x != 0) dir.x = (dir.x > 0) ? 1.f : -1.f;
    if (dir.y != 0) dir.y = (dir.y > 0) ? 1.f : -1.f;
    return dir;
}