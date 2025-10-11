#include "Window.h"
#include <stdio.h>

Window::Window()
{
    width = 800;
    height = 600;
    glfwMajorVersion = 3;
    glfwMinorVersion = 1;

    for (int i = 0; i < 1024; i++) keys[i] = false;
    lastX = 400; // ครึ่งจอเริ่มต้น
    lastY = 300;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight, GLint majorVersion, GLint minorVersion)
{
    width = windowWidth;
    height = windowHeight;
    glfwMajorVersion = majorVersion;
    glfwMinorVersion = minorVersion;

    for (int i = 0; i < 1024; i++) keys[i] = false;
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::initialise()
{
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    mainWindow = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);
    if (!mainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    createCallbacks(); // ผูก callbacks
    glfwSetWindowUserPointer(mainWindow, this);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    return 0;
}

// สำหรับ camera
float Window::getXChange()
{
    float temp = xChange;
    xChange = 0.0f;
    return temp;
}

float Window::getYChange()
{
    float temp = yChange;
    yChange = 0.0f;
    return temp;
}

// internal
void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

// static callback
void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            theWindow->keys[key] = true;
        else if (action == GLFW_RELEASE)
            theWindow->keys[key] = false;
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos;

    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}
