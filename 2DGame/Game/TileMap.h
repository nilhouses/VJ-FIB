#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


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

	// Devuelve si el bloque que se encuentra en la posición dada es sólido
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

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

};


#endif // _TILE_MAP_INCLUDE


