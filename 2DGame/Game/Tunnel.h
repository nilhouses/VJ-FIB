#ifndef _TUNNEL_INCLUDE
#define _TUNNEL_INCLUDE


#include "Enter.h"
#include "TileMap.h"
#include "Camera.h"


class Tunnel : public Enter
{
public:
	Tunnel();
	~Tunnel();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, int sr);

	void setUp();
	void setDown();
	bool getUp() const { return up; }

private:
	bool up = false;
};


#endif // _TUNNEL_INCLUDE

