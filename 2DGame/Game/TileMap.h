#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <unordered_map>
#include "Utils.h"
#include <glm/glm.hpp>

// Tipos de bloque
enum TileType {
	TILE_EMPTY,
	TILE_GROUND,
	TILE_LADDER_GROUND,
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
	void renderFront2() const;
	void free();

	int getTileSize() const { return tileSize; }
	int getBlockSize() const { return blockSize; }
	glm::ivec2 getMapSize() const { return mapSize; }

	// Devuelve si el bloque que se encuentra en la posición dada es sólido
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size);
	bool collisionLadderUp(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionLadderDown(const glm::vec2& pos, const glm::ivec2& size);
	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, float offsetY);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int fallStep);
	bool isOutOfMap(const glm::vec2& pos, const glm::ivec2& size);

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
	GLuint vaoMap, vaoFront, vaoFront2;
	GLuint vboMap, vboFront, vboFront2;
	GLint posLocationMap, posLocationFront, posLocationFront2;
	GLint texCoordLocationMap, texCoordLocationFront, texCoordLocationFront2;
	int nTilesMap, nTilesFront, nTilesFront2;
	int *map, *front, *front2 = nullptr;


	glm::ivec2 position, mapSize, tilesheetSize;	// Posición del mapa, tamaño del mapa (en bloques) y tamaño del tilesheet (en bloques)
	int tileSize, blockSize;						// Tamaño de los bloques del mapa (en píxels) y tamaño de los bloques del tilesheet (en píxels)
	Texture tilesheet;								// Tilesheet (con todos los bloques)
	glm::vec2 tileTexSize;							// Tamaño de los bloques del tilesheet (en coordenadas de textura)
	

	// Lista de tipos de bloque, indexada por el número de bloque en el mapa
	std::unordered_map<int, TileType> tileTypes = {

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
		{ 12, TILE_LADDER_GROUND },
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
		{ 30, TILE_SOLID },
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
		{ 57, TILE_GROUND },
		{ 58, TILE_GROUND },
		{ 59, TILE_GROUND },
		{ 60, TILE_GROUND },
		{ 61, TILE_GROUND },
		{ 62, TILE_GROUND },
		{ 63, TILE_GROUND },
		{ 64, TILE_EMPTY },
		// Fila 5
		{ 65, TILE_EMPTY },
		{ 66, TILE_SOLID },
		{ 67, TILE_SOLID },
		{ 68, TILE_SOLID },
		{ 69, TILE_SOLID },
		{ 70, TILE_SOLID },
		{ 71, TILE_LADDER_GROUND },
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
		{ 144, TILE_EMPTY },
		// Fila 10
		{ 145, TILE_EMPTY },
		{ 146, TILE_EMPTY },
		{ 147, TILE_EMPTY },
		{ 148, TILE_EMPTY },
		{ 149, TILE_EMPTY },
		{ 150, TILE_EMPTY },
		{ 151, TILE_EMPTY },
		{ 152, TILE_EMPTY },
		{ 153, TILE_EMPTY },
		{ 154, TILE_EMPTY },
		{ 155, TILE_EMPTY },
		{ 156, TILE_EMPTY },
		{ 157, TILE_EMPTY },
		{ 158, TILE_EMPTY },
		{ 159, TILE_EMPTY },
		{ 160, TILE_EMPTY },
		// Fila 11
		{ 161, TILE_GROUND },
		{ 162, TILE_GROUND },
		{ 163, TILE_GROUND },
		{ 164, TILE_GROUND },
		{ 165, TILE_GROUND },
		{ 166, TILE_GROUND },
		{ 167, TILE_GROUND },
		{ 168, TILE_GROUND },
		{ 169, TILE_GROUND },
		{ 170, TILE_GROUND },
		{ 171, TILE_GROUND },
		{ 172, TILE_GROUND },
		{ 173, TILE_GROUND },
		{ 174, TILE_GROUND },
		{ 175, TILE_EMPTY },
		{ 176, TILE_EMPTY },
		// Fila 12
		{ 177, TILE_EMPTY },
		{ 178, TILE_EMPTY },
		{ 179, TILE_EMPTY },
		{ 180, TILE_EMPTY },
		{ 181, TILE_EMPTY },
		{ 182, TILE_EMPTY },
		{ 183, TILE_EMPTY },
		{ 184, TILE_EMPTY },
		{ 185, TILE_EMPTY },
		{ 186, TILE_EMPTY },
		{ 187, TILE_EMPTY },
		{ 188, TILE_EMPTY },
		{ 189, TILE_EMPTY },
		{ 190, TILE_EMPTY },
		{ 191, TILE_EMPTY },
		{ 192, TILE_EMPTY },
		// Fila 13
		{ 193, TILE_EMPTY },
		{ 194, TILE_EMPTY },
		{ 195, TILE_EMPTY },
		{ 196, TILE_EMPTY },
		{ 197, TILE_EMPTY },
		{ 198, TILE_EMPTY },
		{ 199, TILE_EMPTY },
		{ 200, TILE_EMPTY },
		{ 201, TILE_EMPTY },
		{ 202, TILE_EMPTY },
		{ 203, TILE_EMPTY },
		{ 204, TILE_EMPTY },
		{ 205, TILE_EMPTY },
		{ 206, TILE_EMPTY },
		{ 207, TILE_EMPTY },
		{ 208, TILE_EMPTY },
		// Fila 14
		{ 209, TILE_EMPTY },
		{ 210, TILE_EMPTY },
		{ 211, TILE_EMPTY },
		{ 212, TILE_EMPTY },
		{ 213, TILE_EMPTY },
		{ 214, TILE_EMPTY },
		{ 215, TILE_EMPTY },
		{ 216, TILE_EMPTY },
		{ 217, TILE_EMPTY },
		{ 218, TILE_EMPTY },
		{ 219, TILE_EMPTY },
		{ 220, TILE_EMPTY },
		{ 221, TILE_EMPTY },
		{ 222, TILE_EMPTY },
		{ 223, TILE_EMPTY },
		{ 224, TILE_EMPTY },
		// Fila 15
		{ 225, TILE_LADDER_GROUND },
		{ 226, TILE_EMPTY },
		{ 227, TILE_EMPTY },
		{ 228, TILE_EMPTY },
		{ 229, TILE_EMPTY },
		{ 230, TILE_EMPTY },
		{ 231, TILE_EMPTY },
		{ 232, TILE_EMPTY },
		{ 233, TILE_EMPTY },
		{ 234, TILE_EMPTY },
		{ 235, TILE_EMPTY },
		{ 236, TILE_EMPTY },
		{ 237, TILE_EMPTY },
		{ 238, TILE_EMPTY },
		{ 239, TILE_EMPTY },
		{ 240, TILE_EMPTY },
		// Fila 16
		{ 241, TILE_LADDER },
		{ 242, TILE_EMPTY },
		{ 243, TILE_EMPTY },
		{ 244, TILE_EMPTY },
		{ 245, TILE_EMPTY },
		{ 246, TILE_EMPTY },
		{ 247, TILE_EMPTY },
		{ 248, TILE_EMPTY },
		{ 249, TILE_EMPTY },
		{ 250, TILE_EMPTY },
		{ 251, TILE_EMPTY },
		{ 252, TILE_EMPTY },
		{ 253, TILE_EMPTY },
		{ 254, TILE_EMPTY },
		{ 255, TILE_EMPTY },
		{ 256, TILE_EMPTY },
		// Fila 17
		{ 257, TILE_LADDER },
		{ 258, TILE_EMPTY },
		{ 259, TILE_EMPTY },
		{ 260, TILE_EMPTY },
		{ 261, TILE_EMPTY },
		{ 262, TILE_EMPTY },
		{ 263, TILE_EMPTY },
		{ 264, TILE_EMPTY },
		{ 265, TILE_EMPTY },
		{ 266, TILE_EMPTY },
		{ 267, TILE_EMPTY },
		{ 268, TILE_EMPTY },
		{ 269, TILE_EMPTY },
		{ 270, TILE_EMPTY },
		{ 271, TILE_EMPTY },
		{ 272, TILE_EMPTY },
	};

};


#endif // _TILE_MAP_INCLUDE
