#ifndef _CLEVER_INCLUDE
#define _CLEVER_INCLUDE

#include "Enemy.h"
#include "Player.h"
#include "Pipe.h"

class Clever : public Enemy
{
public:
	Clever();
	~Clever();

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Camera* c, bool movingRight) override;
	void update(int deltaTime) override;
	void die() override;
	void changeDirection() override;
	void render() override;

	// Funciones del Clever
	void setPlayerTarget(Player* p) { playerTarget = p; }
	void setPipes(const vector<Pipe*>& p) { pipes = p; }
	pair<Pipe*, int> getPipeEntryAt() const;
	void setVisible(bool v) { visible = v; }
	bool isVisible() const { return visible; }

private:
	Player* playerTarget = nullptr;
	bool isClimbing = false;
	bool wasInAir = false;
	bool wasClimbing = false;
	vector<Pipe*> pipes;
	Pipe* currentPipe = nullptr;
	bool inPipe = false;
	bool visible = true;
	int verticalCooldown = 0;
	static const int VERTICAL_COOLDOWN = 1000; // 1 segundo
};


#endif // _CLEVER_INCLUDE