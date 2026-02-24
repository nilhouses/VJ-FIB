#ifndef _PLAYSCENE_INCLUDE
#define _PLAYSCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Scene.h"

class PlayScene : public Scene
{
public:
    // Pasamos el nivel para el constructor para que sepa que .txt cargar
    PlayScene(int levelNumber);
    ~PlayScene();

    void init() override;
    void update(int deltaTime) override;
    void render() override;

private:
    void initShaders();	// Carga el VS y FS y los linkea al texProgram

private:
    TileMap* map;				// El mapa de tiles del nivel
    Player* player;				// El jugador
    ShaderProgram texProgram;	// El programa de shaders para renderizar el mapa y el jugador
    float currentTime;			// El tiempo actual del juego
    glm::mat4 projection;		// La matriz de proyección para renderizar el mapa y el jugador

    int level;
};

#endif // _PLAYSCENE_INCLUDE