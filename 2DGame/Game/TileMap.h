#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include "Utils.h"

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
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	~TileMap();

	void renderBase() const;
	void renderFront() const;
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }

	// Devuelve si el bloque que se encuentra en la posición dada es sólido
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionLadderUp(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionLadderDown(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, float offsetY);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int fallStep);

	void print() const; // Función de depuración para imprimir el mapa en consola
	
private:
	bool loadLevel(const string& levelFile);
	void prepareLayerArray(const glm::vec2& minCoords, ShaderProgram& program, int* layer, GLuint& vao, GLuint& vbo, int& nTiles, GLint& posLocation, GLint& texCoordLocation);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

	TileType getTileType(int tile) const;
	bool isGround(int x, int y);
	bool isSolid(int x, int y);
	bool isLadder(int x, int y);

private:
	// Para crear una nueva capa añadir un elemento nuevo a esta lista y modificar loadLevel() + prepareArrays() + render()
	GLuint vaoMap, vaoFront;
	GLuint vboMap, vboFront;
	GLint posLocationMap, posLocationFront;
	GLint texCoordLocationMap, texCoordLocationFront;
	int nTilesMap, nTilesFront;
	int *map, *front;


	glm::ivec2 position, mapSize, tilesheetSize;	// Posición del mapa, tamaño del mapa (en bloques) y tamaño del tilesheet (en bloques)
	int tileSize, blockSize;						// Tamaño de los bloques del mapa (en píxels) y tamaño de los bloques del tilesheet (en píxels)
	Texture tilesheet;								// Tilesheet (con todos los bloques)
	glm::vec2 tileTexSize;							// Tamaño de los bloques del tilesheet (en coordenadas de textura)
	

	// Lista de tipos de bloque, indexada por el número de bloque en el mapa
	std::unordered_map<int, TileType> tileTypes = {
		// Test sprites
		/*
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
		*/

		// Tileset definitivo
		{ 0, TILE_EMPTY },
		// Fila 1
		{ 1, TILE_GROUND },
		{ 2, TILE_GROUND },
		{ 3, TILE_GROUND },
		{ 4, TILE_GROUND },
		{ 5, TILE_GROUND },
		{ 6, TILE_GROUND },
		{ 7, TILE_GROUND },
		{ 8, TILE_EMPTY },
		{ 9, TILE_EMPTY },
		{ 10, TILE_EMPTY },
		{ 11, TILE_EMPTY },
		{ 12, TILE_LADDER },
		{ 13, TILE_GROUND },
		{ 14, TILE_GROUND },
		{ 15, TILE_GROUND },
		{ 16, TILE_GROUND },
		// Fila 2
		{ 17, TILE_EMPTY },
		{ 18, TILE_EMPTY },
		{ 19, TILE_EMPTY },
		{ 20, TILE_EMPTY },
		{ 21, TILE_EMPTY },
		{ 22, TILE_EMPTY },
		{ 23, TILE_EMPTY },
		{ 24, TILE_EMPTY },
		{ 25, TILE_EMPTY },
		{ 26, TILE_EMPTY },
		{ 27, TILE_EMPTY },
		{ 28, TILE_LADDER },
		{ 29, TILE_EMPTY },
		{ 30, TILE_EMPTY },
		{ 31, TILE_EMPTY },
		{ 32, TILE_EMPTY },
		// Fila 3
		{ 33, TILE_EMPTY },
		{ 34, TILE_EMPTY },
		{ 35, TILE_EMPTY },
		{ 36, TILE_EMPTY },
		{ 37, TILE_EMPTY },
		{ 38, TILE_EMPTY },
		{ 39, TILE_EMPTY },
		{ 40, TILE_EMPTY },
		{ 41, TILE_EMPTY },
		{ 42, TILE_EMPTY },
		{ 43, TILE_EMPTY },
		{ 44, TILE_LADDER },
		{ 45, TILE_EMPTY },
		{ 46, TILE_EMPTY },
		{ 47, TILE_EMPTY },
		{ 48, TILE_EMPTY },
		// Fila 4
		{ 49, TILE_EMPTY },
		{ 50, TILE_EMPTY },
		{ 51, TILE_EMPTY },
		{ 52, TILE_EMPTY },
		{ 53, TILE_EMPTY },
		{ 54, TILE_SOLID },
		{ 55, TILE_SOLID },
		{ 56, TILE_EMPTY },
		{ 57, TILE_EMPTY },
		{ 58, TILE_EMPTY },
		{ 59, TILE_EMPTY },
		{ 60, TILE_EMPTY },
		{ 61, TILE_EMPTY },
		{ 62, TILE_EMPTY },
		{ 63, TILE_EMPTY },
		{ 64, TILE_EMPTY },
		// Fila 5
		{ 65, TILE_EMPTY },
		{ 66, TILE_SOLID },
		{ 67, TILE_SOLID },
		{ 68, TILE_SOLID },
		{ 69, TILE_SOLID },
		{ 70, TILE_SOLID },
		{ 71, TILE_LADDER },
		{ 72, TILE_EMPTY },
		{ 73, TILE_EMPTY },
		{ 74, TILE_EMPTY },
		{ 75, TILE_EMPTY },
		{ 76, TILE_EMPTY },
		{ 77, TILE_EMPTY },
		{ 78, TILE_EMPTY },
		{ 79, TILE_EMPTY },
		{ 80, TILE_EMPTY },
		// Fila 6
		{ 81, TILE_EMPTY },
		{ 82, TILE_SOLID },
		{ 83, TILE_EMPTY },
		{ 84, TILE_SOLID },
		{ 85, TILE_EMPTY },
		{ 86, TILE_SOLID },
		{ 87, TILE_LADDER },
		{ 88, TILE_EMPTY },
		{ 89, TILE_EMPTY },
		{ 90, TILE_EMPTY },
		{ 91, TILE_EMPTY },
		{ 92, TILE_EMPTY },
		{ 93, TILE_EMPTY },
		{ 94, TILE_EMPTY },
		{ 95, TILE_EMPTY },
		{ 96, TILE_EMPTY },
		// Fila 7
		{ 97, TILE_EMPTY },
		{ 98, TILE_SOLID },
		{ 99, TILE_EMPTY },
		{ 100, TILE_EMPTY },
		{ 101, TILE_EMPTY },
		{ 102, TILE_SOLID },
		{ 103, TILE_LADDER },
		{ 104, TILE_EMPTY },
		{ 105, TILE_EMPTY },
		{ 106, TILE_EMPTY },
		{ 107, TILE_EMPTY },
		{ 108, TILE_EMPTY },
		{ 109, TILE_EMPTY },
		{ 110, TILE_EMPTY },
		{ 111, TILE_EMPTY },
		{ 112, TILE_EMPTY },
		// Fila 8
		{ 113, TILE_EMPTY },
		{ 114, TILE_SOLID },
		{ 115, TILE_SOLID },
		{ 116, TILE_SOLID },
		{ 117, TILE_SOLID },
		{ 118, TILE_SOLID },
		{ 119, TILE_SOLID },
		{ 120, TILE_EMPTY },
		{ 121, TILE_EMPTY },
		{ 122, TILE_EMPTY },
		{ 123, TILE_EMPTY },
		{ 124, TILE_EMPTY },
		{ 125, TILE_EMPTY },
		{ 126, TILE_EMPTY },
		{ 127, TILE_EMPTY },
		{ 128, TILE_EMPTY },
		// Fila 9
		{ 129, TILE_EMPTY },
		{ 130, TILE_SOLID },
		{ 131, TILE_SOLID },
		{ 132, TILE_SOLID },
		{ 133, TILE_SOLID },
		{ 134, TILE_SOLID },
		{ 135, TILE_SOLID },
		{ 136, TILE_EMPTY },
		{ 137, TILE_EMPTY },
		{ 138, TILE_EMPTY },
		{ 139, TILE_EMPTY },
		{ 140, TILE_EMPTY },
		{ 141, TILE_EMPTY },
		{ 142, TILE_EMPTY },
		{ 143, TILE_EMPTY },
		{ 144, TILE_EMPTY }
	};

};


#endif // _TILE_MAP_INCLUDE
