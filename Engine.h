#pragma once
#include <GLFW/glfw3.h>

class Engine
{
public:
    Engine();
    bool init();
    void run();
    void shutdown();

private:
    GLFWwindow* window;
    int width;
    int height;
    bool fullscreen;
    bool perspective;
    int fps;

    float clearR, clearG, clearB;
    double lastTime;

    // dla myszy
    double lastMouseX, lastMouseY;
    float cameraAngleX, cameraAngleY;
    bool firstMouse;

    void processInput();
    void update();
    void render();
    void setProjection();
    void processMouse();
};
