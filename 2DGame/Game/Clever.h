#ifndef _CLEVER_INCLUDE
#define _CLEVER_INCLUDE

#include "Enemy.h"
#include "Player.h"
#include "Pipe.h"
#include "Tunnel.h"

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
	void notifyTunnelEntry(Tunnel* t);
	bool isInTunnel() const { return inTunnel; }

	// Funciones del Clever
	void setPlayerTarget(Player* p) { playerTarget = p; }
	void setPipes(const vector<Pipe*>& p) { pipes = p; }
	pair<Pipe*, int> getPipeEntryAt() const;
	void setVisible(bool v) { visible = v; }
	bool isVisible() const { return visible; }
	bool centeredOn(const glm::vec4& bbox) const;

private:
	Player* playerTarget = nullptr;
	bool isClimbing = false;
	int ladderDirection = 0; // -1 arriba, +1 abajo
	bool wasInAir = false;
	bool wasClimbing = false;
	// Escalera
	static const int VERTICAL_COOLDOWN = 1000; // 1 segundo, para evitar que el clever suba y baje por la misma escalera o pipe repetidamente
	// Pipes
	vector<Pipe*> pipes = vector<Pipe*>();
	Pipe* currentPipe = nullptr;
	bool inPipe = false;
	bool visible = true;
	int verticalCooldown = 0;
	// Tuneles
	Tunnel* currentTunnel = nullptr;
	Tunnel* lastUsedTunnel = nullptr;
	bool inTunnel = false;
	bool tunnelTeleported = false;
	static constexpr float TUNNEL_DURATION = 1000.f; // ms, igual que el player
	static constexpr float TUNNEL_CLEAR_DISTANCE = 16.f; // N?mero de px que hace falta recorrer para no volver a usar el mismo t?nel, evita el t?pico bucle de entrar y salir del t?nel constantemente, que no se usar? casi nunca pero depende del nivel
	Camera* cam = nullptr;
	float startAnimTimer = 0.f;
};


#endif // _CLEVER_INCLUDE