#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"

enum class Type {
	PLAYER,
	KEY,
	BARREL,
	ENEMY,
	BULLET,
	LIFE,
	SPEEDBOOST,
	GUN,
	PLATFORM,
	PIPE,
	ENTER,
	ACID,
	ENTERARROW,
	NONE
};

class Entity
{
public:
	Entity(Type t);
	virtual ~Entity();

	// Todas las entidades se inicializan con la misma función init. En este punto la posición es fija respecto al mapa de tiles
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const string& spritesheetDir, const glm::vec2& quadSize, const glm::vec2& sizeInSpritesheet, Camera* c);
	
	// La función de update es abstracta y cada entidad tiene su propia implementación
	virtual void update(int deltaTime) = 0;
	
	// La función de render es común para todas porque delega la lógica de renderizado al sprite, que es el mismo para todas las entidades. De esta forma, cada entidad solo tiene que implementar la función de update con su propia lógica.
	virtual void render();

	virtual void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition() const { return pos; }

	void setTileMap(TileMap* tileMap){map = tileMap;}
	// Función para activar o desactivar la entidad. Si una entidad está desactivada, no se renderiza ni se actualiza, pero sigue existiendo en el juego (por ejemplo, un enemigo que ha sido derrotado pero que sigue existiendo en el mapa).
	void activate() { active = true; }
	void deactivate() { active = false; }
	// Devuelve si está activo o no
	bool isActive() const { return active; }
	// Devuelve el tipo de la entidad
	Type getType() const { return type; }
	// Devuelve el bounding box de la entidad para detectar colisiones.
	virtual glm::vec4 getBoundingBox() const;
	// Devuelve el tamaño de la entidad
	const glm::ivec2& getSize() const { return size; }

	int getRoom() const { return room; }
	virtual void setRoomInt(int r) { room = r; }
	virtual bool isEnemy() const { return false; }

	void printDebugInfo() const; // Función para imprimir información de depuración sobre la entidad (posición, tipo, estado, etc.)
	
protected:
	TileMap* map = nullptr;					    										// Mapa de tiles para detectar colisiones
	glm::ivec2 tileMapDispl = glm::ivec2(0,0), pos = glm::ivec2(0,0);					// Posición de la entidad en el mapa de tiles (px)
	Texture spritesheet = Texture();
	Sprite *sprite = nullptr;
	bool active = true;																	// Indica si la entidad está activa (visible y actualizable) o no
	Type type = Type::NONE;																// Aquí guardamos qué es (Player, Key, etc.)
	int room = -1;
	float itemTimer = 0.f;
	glm::ivec2 size = glm::ivec2(32, 32);   // Tamaño del bounding box por defecto (se asume que es un cuadrado de 32x32 píxeles, pero cada entidad puede sobreescribir esta función para devolver un bounding box diferente
};

#endif // _ENTITY_INCLUDE