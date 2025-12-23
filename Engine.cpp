#include "Engine.h"
#include <GL/gl.h>
#include <iostream>
#include <cmath>

Engine::Engine()
{
    window = nullptr;
    width = 800;
    height = 600;
    fullscreen = false;
    perspective = true;
    fps = 60;

    clearR = 0.1f;
    clearG = 0.1f;
    clearB = 0.15f;

    lastTime = 0.0;

    lastMouseX = 0.0;
    lastMouseY = 0.0;
    cameraAngleX = 0.0f;
    cameraAngleY = 0.0f;
    firstMouse = true;
}

bool Engine::init()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, "Engine 3D - GLFW", monitor, nullptr);
    if (!window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // V-sync

    glEnable(GL_DEPTH_TEST);
    glClearColor(clearR, clearG, clearB, 1.0f);

    lastTime = glfwGetTime();

    // mysz
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

void Engine::run()
{
    const double frameTime = 1.0 / fps;

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;

        if (delta >= frameTime)
        {
            lastTime = currentTime;

            processInput();
            processMouse();
            update();
            render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    shutdown();
}

void Engine::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        perspective = true;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        perspective = false;

    // jak f to fullscreen
    static bool fKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed)
    {
        fKeyPressed = true;
        fullscreen = !fullscreen;

        int w = width, h = height;
        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

        glfwDestroyWindow(window);
        window = glfwCreateWindow(w, h, "Okno", monitor, nullptr);
        glfwMakeContextCurrent(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        fKeyPressed = false;
}

void Engine::processMouse()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.1f;
    cameraAngleX += (xpos - lastMouseX) * sensitivity;
    cameraAngleY += (ypos - lastMouseY) * sensitivity;

    lastMouseX = xpos;
    lastMouseY = ypos;

    if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
    if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;
}

void Engine::update()
{
    // logika na razie pusta
}

void Engine::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setProjection();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // obrót kamery
    glRotatef(-cameraAngleY, 1.0f, 0.0f, 0.0f);
    glRotatef(-cameraAngleX, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -5.0f);

    // kolorowanie trójk¹ta
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex3f(-1, -1, 0);
    glColor3f(0, 1, 0); glVertex3f(1, -1, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 1, 0);
    glEnd();
}

void Engine::setProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / height;

    if (perspective)
    {
        float fov = 60.0f * 3.1415926f / 180.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;
        float top = tan(fov / 2.0f) * zNear;
        float right = top * aspect;
        glFrustum(-right, right, -top, top, zNear, zFar);
    }
    else
    {
        float orthoSize = 5.0f;
        glOrtho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, -10.0f, 10.0f);
    }
}

void Engine::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
