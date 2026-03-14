#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete[] map;
	free();
}

void TileMap::renderBase() const
{
	if (nTilesMap == 0) return;

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();

	// Print base layer
	glBindVertexArray(vaoMap);
	glEnableVertexAttribArray(posLocationMap);
	glEnableVertexAttribArray(texCoordLocationMap);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTilesMap);

	glDisable(GL_TEXTURE_2D);
}

void TileMap::renderFront() const
{
	if (nTilesFront == 0) return;
	
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();

	// Print front layer
	glBindVertexArray(vaoFront);
	glEnableVertexAttribArray(posLocationFront);
	glEnableVertexAttribArray(texCoordLocationFront);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTilesFront);

	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteVertexArrays(1, &vaoMap);
	glDeleteVertexArrays(1, &vaoFront);
	glDeleteBuffers(1, &vboMap);
	glDeleteBuffers(1, &vboFront);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	// Read base
	getline(fin, line);
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		getline(fin, line);
		sstream.clear();
		sstream.str(line);
		for (int i = 0; i < mapSize.x; i++)
		{
			sstream >> tile;
			if (sstream.peek() == ',')
				sstream.ignore();
			map[j * mapSize.x + i] = tile + 1;
		}
	}

	// Read front
	getline(fin, line);
	front = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		getline(fin, line);
		sstream.clear();
		sstream.str(line);
		for (int i = 0; i < mapSize.x; i++)
		{
			sstream >> tile;
			if (sstream.peek() == ',')
				sstream.ignore();
			front[j * mapSize.x + i] = tile + 1;
		}
	}

	fin.close();

	return true;
}


void TileMap::prepareLayerArray(const glm::vec2& minCoords, ShaderProgram& program, int* layer, GLuint& vao, GLuint& vbo, int& nTiles, GLint& posLocation, GLint& texCoordLocation)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = layer[j * mapSize.x + i];
			if (tile != 0)
			{
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}
	
	if (nTiles == 0) return;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}


void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	prepareLayerArray(minCoords, program, map, vaoMap, vboMap, nTilesMap, posLocationMap, texCoordLocationMap);
	prepareLayerArray(minCoords, program, front, vaoFront, vboFront, nTilesFront, posLocationFront, texCoordLocationFront);
}


// DETECTORES DE COLISIONES
bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size)
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (isSolid(x, y))
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size)
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (isSolid(x, y))
			return true;
	}

	return false;
}


bool TileMap::collisionLadderUp(const glm::vec2& pos, const glm::ivec2& size)
{
	int x, y0, y1;

	int centerX = (int)pos.x + size.x / 2;
	x = centerX / tileSize;
	y0 = (int)pos.y / tileSize;
	y1 = ((int)pos.y + size.y - 1) / tileSize;
	
	for (int y = y0; y <= y1; y++)
	{
		if (isLadder(x, y) && Utils::isCentered(centerX)) {
			return true;
		}
	}
	return false;
}

bool TileMap::collisionLadderDown(const glm::vec2& pos, const glm::ivec2& size)
{
	int x, y;

	int centerX = (int)pos.x + size.x / 2;
	x = centerX / tileSize;
	y = ((int)pos.y + size.y) / tileSize;
	if (isLadder(x, y) && Utils::isCentered(centerX))
	{
		return true;
	}

	return false;
}


bool TileMap::collisionDown(const glm::ivec2 &pos, const glm::ivec2 &size, float offsetY)
{
	int x0, x1, y, ypx;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	ypx = (pos.y + size.y + (int)offsetY);
	y =  ypx / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (isGround(x, y) && (ypx % blockSize >= 0) && (ypx % blockSize <= offsetY))
			return true;
	}
	return false;
}


bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, int fallStep)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (isGround(x, y))
		{
			if(*posY - tileSize * y + size.y <= fallStep)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

/* CONSULTORAS DE TIPOS DE TILES [PRIVATE] */

// Devuelve el tipo de bloque del bloque dado
TileType TileMap::getTileType(int tile) const
{
	auto it = tileTypes.find(tile);
	if (it != tileTypes.end())
		return it->second;
	else
		return TILE_EMPTY;
}

// Devuelve si el bloque del índice x,y es suelo
bool TileMap::isGround(int x, int y)
{
	int tileBase = map[y * mapSize.x + x];
	int tileFront = front[y * mapSize.x + x];
	return getTileType(tileBase) != TILE_EMPTY || getTileType(tileFront) != TILE_EMPTY;
}

// Devuelve si el bloque del índice x,y es sólido
bool TileMap::isSolid(int x, int y)
{
	int tileBase = map[y * mapSize.x + x];
	int tileFront = front[y * mapSize.x + x];
	return getTileType(tileBase) == TILE_SOLID || getTileType(tileFront) == TILE_SOLID;
}

// Devuelve si el bloque del índice x,y es una escalera
bool TileMap::isLadder(int x, int y) {
	int tile = front[y * mapSize.x + x];
	return getTileType(tile) == TILE_LADDER;
}




void TileMap::print() const
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			cout << map[j * mapSize.x + i] << " ";
		}
		cout << endl;
	}

	cout << "---------------------" << endl;

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			cout << front[j * mapSize.x + i] << " ";
		}
		cout << endl;
	}
}