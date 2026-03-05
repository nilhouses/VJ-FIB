#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"

// Definimos 4 tipos de animaciones para el dummy
enum DummyAnims
{
    MOVE_LEFT, MOVE_RIGHT, DIE, NUM_ANIMS
};

Enemy::Enemy(EnemyType t) : Entity(Type::ENEMY), eType(t) {
    deathTimer = 0.f;
}

void Enemy::incrRight() { pos.x += speed; }
void Enemy::incrLeft() { pos.x -= speed; }

// Los hijos las sobreescriben
void Enemy::changeDirection() {}
void Enemy::die() {}
