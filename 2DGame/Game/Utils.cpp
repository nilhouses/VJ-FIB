
#include "Utils.h"

#define TILE_SIZE 32

bool Utils::isCentered(int x) {
	return (x % TILE_SIZE > (TILE_SIZE / 4 - 2)) && (x % TILE_SIZE < (TILE_SIZE * 3 / 4 + 2));
}