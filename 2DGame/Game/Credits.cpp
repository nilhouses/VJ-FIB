#include "Credits.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

// Colores y constantes
static const glm::vec4 COL_YELLOW = { 1.f,  1.f,  0.f,  1.f };
static const glm::vec4 COL_BLACK = { 0.f,  0.f,  0.f,  1.f };
static const glm::vec4 COL_LIGHT_BLUE = { 0.4f, 0.8f, 1.0f, 1.f };

static const int   TITLE_SIZE = 40;
static const int   SECTION_SIZE = 20;
static const int   NAME_SIZE = 22;
static const float TITLE_MARGIN = 0.f;
static const float SECTION_MARGIN = 50.f;  // Espacio por encima de cada cabecera
static const float NAME_MARGIN = 10.f;  // Espacio por encima de cada nombre (o sección, si es el primer nombre de una sección)
static const float LINE_SPACING = 10.f;  // Espacio entre líneas (tanto entre nombres como entre secciones)
static const float START_Y = 80.f;

Credits::Credits() : Scene(SceneType::CREDITS) {
    background = nullptr;
}

Credits::~Credits() {
    if (background != nullptr) delete background;
}


CreditEntry Credits::makeTitle(const string& label) {
    return { label, TITLE_SIZE, COL_YELLOW, TITLE_MARGIN };
}
CreditEntry Credits::makeSection(const string& label) {
    return { label, SECTION_SIZE, COL_LIGHT_BLUE, SECTION_MARGIN };
}
CreditEntry Credits::makeName(const string& label) {
    return { label, NAME_SIZE, COL_BLACK, NAME_MARGIN };
}

void Credits::buildCredits() {
    text_entries = {
        makeTitle("CREDITS"),

        makeSection("DEVELOPERS"),
        makeName("Alejandro Ruiz Paton"),
        makeName("Nil Casas Duatis"),

        makeSection("ART DESIGN"),
        makeName("Adragong3"),

        makeSection("SOUNDTRACK"),
        makeName("Alejandro Ruiz Paton"),

        makeSection("SOUND EFFECTS:"),
        makeName("Nil Casas Duatis"),
        makeName("pixabay.com"),
        makeName("freesound.org"),

        makeSection("LEVEL DESIGN"),
        makeName("Alejandro Ruiz Paton"),
        makeName("Nil Casas Duatis"),

        makeSection("MADE WITH OpenGL"),

        makeSection("THANKS FOR PLAYING!"),
    };

    // Calcular las Y automáticamente
    float cursor = START_Y;
    text_entryY.clear();
    for (const auto& e : text_entries) {
        cursor += e.marginTop;
        text_entryY.push_back(cursor);
        cursor += e.fontSize + LINE_SPACING;
    }

    totalContentHeight = max(0.f, cursor - float(SCREEN_HEIGHT) + 150.f);
}

void Credits::init() {
    initShaders();

    texBackground.loadFromFile("images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);
    glm::vec2 geomBG[2] = { {0.f, 0.f}, {float(SCREEN_WIDTH), float(SCREEN_HEIGHT)} };
    glm::vec2 texCoords[2] = { {0.f, 0.f}, {1.f, 1.f} };
    background = TexturedQuad::createTexturedQuad(geomBG, texCoords, texProgram);

    if (!text.init("fonts/PressStart2P.ttf"))
        cout << "Could not load pixel font!!!" << endl;

    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

    buildCredits();
}

void Credits::update(int deltaTime) {
    timer += deltaTime;

    if (Game::instance().getKey(GLFW_KEY_ESCAPE) || Game::instance().getKey(GLFW_KEY_B))
        Game::instance().changeState(MAIN_MENU);
    
    if (startDelay > 0.f) {
        startDelay -= deltaTime;
    } else if (!scrollDone) {
        scrollOffset += scrollSpeed * deltaTime;
        if (scrollOffset >= totalContentHeight) {
            scrollOffset = totalContentHeight;
            scrollDone = true;
        }
    }
    // Fade del "PRESS B" cuando acaba el scroll
    if (scrollDone) {
        fadeTimer += deltaTime;
        fadeAlpha = min(1.f, fadeTimer / 1000.f);  // 1 segundo
    }
}

void Credits::render() {
    glm::mat4 modelview;
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

    // Fondo
    modelview = glm::mat4(1.f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    background->render(texBackground);

    // Créditos
    for (int i = 0; i < (int)text_entries.size(); ++i) {
        float y = text_entryY[i] - scrollOffset;
        if (y < -50.f || y > float(SCREEN_HEIGHT) + 50.f) continue;
        text.render(text_entries[i].label, glm::vec2(100.f, y), text_entries[i].fontSize, text_entries[i].color, projection);
    }

    // Volver atrás
    if (scrollDone) {
        float animY = float(SCREEN_HEIGHT) - 60.f + 5.f * sin(timer / 300.f);
        text.render("PRESS 'B' TO GO BACK", glm::vec2(165, animY), 16, glm::vec4(0.f, 0.f, 0.f, fadeAlpha), projection);
    }
}

void Credits::initShaders() {
    Shader vShader, fShader;
    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    texProgram.bindFragmentOutput("outColor");
}