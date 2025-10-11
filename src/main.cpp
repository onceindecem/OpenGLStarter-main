#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"
#include "Libs/stb_image.h"
#include "Libs/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";
// Fragment Shader
static const char *fShader = "Shaders/shader.frag";

// แสงสีเหลือง
glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 0.0f);

// กล้อง
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -90.0f, 0.0f, 5.0f, 0.1f);

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void CreateOBJ()
{
    Mesh *obj1 = new Mesh();
    bool loaded = obj1->CreateMeshFromOBJ("Models/Duck.obj");

    if (loaded)
    {
        for (int i = 0; i < 10; i++)
        {
            meshList.push_back(obj1);
        }
    }
    else
    {
        std::cout << "OBJ failed to load" << std::endl;
    }
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateOBJ();
    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f);

    // โหลด Texture
    unsigned int texture1;
    int width1, height1, nrChannels1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data1 = stbi_load("Textures/uvmap.png", &width1, &height1, &nrChannels1, 0);
    if (!data1)
    {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n";
    }
    else
    {
        GLenum format = GL_RGB;
        if (nrChannels1 == 1)
            format = GL_RED;
        else if (nrChannels1 == 3)
            format = GL_RGB;
        else if (nrChannels1 == 4)
            format = GL_RGBA;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data1);
    }

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        // รับค่าปุ่มและเมาส์ไปควบคุมกล้อง
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetUniformLocation("model");
        uniformView = shaderList[0].GetUniformLocation("view");
        uniformProjection = shaderList[0].GetUniformLocation("projection");

        glm::mat4 view = camera.calculateViewMatrix();
        glm::vec3 cameraPos = camera.getCameraPosition();

        glm::vec3 pyramidPosition[] = {
            glm::vec3(0.0f, 0.0f, -2.5f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)};

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pyramidPosition[i]);
            model = glm::scale(model, glm::vec3(0.8f, 0.8f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));

            GLint uniformTexture1 = shaderList[0].GetUniformLocation("texture1");
            glUniform1i(uniformTexture1, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);

            glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
            glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, (GLfloat *)&lightColour);
            glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);

            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
