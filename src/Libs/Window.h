#ifndef WINDOW____H
#define WINDOW____H

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight, GLint majorVersion, GLint minorVersion);
    ~Window();

    int initialise();

    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    GLFWwindow *getWindow() { return mainWindow; }

    float getXChange();
    float getYChange();
    bool* getKeys() { return keys; }

private:
    GLFWwindow *mainWindow;
    GLint glfwMajorVersion, glfwMinorVersion;
    GLint width, height;
    GLint bufferWidth, bufferHeight;

    // 👇 ตัวแปรสำหรับกล้อง
    bool keys[1024];
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    // internal functions
    void createCallbacks();

    // static callback wrappers
    static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};

#endif
