#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <unordered_map>

// Tipos de bloque
enum TileType {
	TILE_EMPTY,
	TILE_GROUND,
	TILE_LADDER,
	TILE_SOLID
};


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }

	// Devuelve si el bloque que se encuentra en la posición dada es sólido
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size);
	bool collisionLadderUp(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionLadderDown(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	
	TileType getTileType(int tile) const;
	bool isGround(int x, int y);
	bool isSolid(int x, int y);
	bool isLadder(int x, int y);

private:
	GLuint vao;										// VAO del mapa de bloques
	GLuint vbo;
	GLint posLocation, texCoordLocation;			
	int nTiles;										// Número de bloques a dibujar
	glm::ivec2 position, mapSize, tilesheetSize;	// Posición del mapa, tamaño del mapa (en bloques) y tamaño del tilesheet (en bloques)
	int tileSize, blockSize;						// Tamaño de los bloques del mapa (en píxels) y tamaño de los bloques del tilesheet (en píxels)
	Texture tilesheet;								// Tilesheet (con todos los bloques)
	glm::vec2 tileTexSize;							// Tamaño de los bloques del tilesheet (en coordenadas de textura)
	int *map;

	// Lista de tipos de bloque, indexada por el número de bloque en el mapa
	std::unordered_map<int, TileType> tileTypes = {
		{ 0, TILE_EMPTY },
		{ 1, TILE_GROUND },
		{ 2, TILE_EMPTY },
		{ 3, TILE_GROUND },
		{ 4, TILE_EMPTY },
		{ 5, TILE_LADDER },
		{ 6, TILE_LADDER },
		{ 7, TILE_LADDER },
		{ 8, TILE_GROUND },
		{ 9, TILE_SOLID },
		{ 10, TILE_EMPTY },
		{ 11, TILE_EMPTY },
		{ 12, TILE_GROUND },
		{ 13, TILE_EMPTY },
		{ 14, TILE_EMPTY },
		{ 15, TILE_EMPTY },
		{ 16, TILE_GROUND }
	};
};


#endif // _TILE_MAP_INCLUDE


