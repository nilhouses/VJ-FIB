#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Entity.h"

enum class EnemyType {
    DUMMY,
    CLEVER,
    SHOOTER
};

class Enemy : public Entity
{
public:
    Enemy(EnemyType t);
    virtual ~Enemy() {}

    // Funciones a implementar por cada tipo de enemigo
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c) = 0; // Cada enemigo tiene sus animaciones, por lo que cada uno se inicializa de forma diferente
    virtual void update(int deltaTime) override = 0;                                              // Cada enemigo se mueve distinto
    virtual void changeDirection();                                                               // Requiere el cambio de sprite
    virtual void die();

    // Getters
    bool isEnemy() const override { return true; }
    EnemyType getEnemyType() const { return eType; }
    int getSpeed() const { return speed; }
    bool isDying() const { return dying; }
    
    // Setters
    void setTileMap(TileMap* tileMap) { map = tileMap; }
    void setSpeed(int s) { speed = s; }
    void setOnGround(bool b) { onGround = b; }

	// Funciones comunes a todos los enemigos
    void incrRight();
    void incrLeft();

protected:
    EnemyType eType;                                    // Tipos de enemigo
    bool dying = false;                                 // Para poder desactivar colisiones con otras entidades
    float deathTimer;                                   // Para controlar el tiempo que dura la animaci�n de muerte antes del deactivate
	int speed = 1;                                      // Todo enemigo tiene una velocidad de movimiento en px. (Por Defecto 1)
	int fallStep = 6;                                   // Px por ca�da (Por defecto 6)
	int deathDuration = 500;                            // Animaci�n muerte en ms (Por defecto 500ms)
    bool movingRight = true;                            // Para controlar la direcci�n del movimiento (true = right, false = left)
    glm::ivec2 size = glm::ivec2(32, 32);               // Tendr�n todos (32, 64?)
    bool onGround = false;
};

#endif // _ENEMY_INCLUDE