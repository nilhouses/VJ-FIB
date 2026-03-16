#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Enemy.h"

Enemy::Enemy(EnemyType t) : Entity(Type::ENEMY), eType(t) {
    deathTimer = 0.f;
}

void Enemy::incrRight() { pos.x += speed; }
void Enemy::incrLeft() { pos.x -= speed; }

// Los hijos las sobreescriben
void Enemy::changeDirection() {}
void Enemy::die() {}

void Enemy::center() {
	pos.x = ((pos.x + map->getBlockSize() / 2) / map->getBlockSize()) * map->getBlockSize();
}