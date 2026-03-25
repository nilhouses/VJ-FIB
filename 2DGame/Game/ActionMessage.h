// ActionMessage.h
#ifndef _ACTION_MESSAGE_INCLUDE
#define _ACTION_MESSAGE_INCLUDE

#include <string>
#include <glm/glm.hpp>

class Text;

class ActionMessage {
public:
    void init(Text* textPtr);
    void update(int deltaTime);
    void render(const glm::mat4& projection);
    void show(const std::string& msg, float durationMs = 2500.f);
    bool isActive() const { return active; }

private:
    Text* text;
    std::string currentMsg;
    float timer;
    float alpha;
    bool active;
};

#endif  // _ACTION_MESSAGE_INCLUDE