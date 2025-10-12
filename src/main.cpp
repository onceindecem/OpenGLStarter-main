#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <filesystem>

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
glm::vec3 lightColour = glm::vec3(0.0f, 0.0f, 0.0f);
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
    if (!obj1->CreateMeshFromOBJ("Models/model_Batman Rubber Duck_20210208_191154320.obj"))
    {
        std::cout << "OBJ failed to load" << std::endl;
    }
    else
    {
        meshList.push_back(obj1); // เพิ่มลง meshList แค่ครั้งเดียว
    }
    Mesh *Earth = new Mesh();
    if (!Earth->CreateMeshFromOBJ("Models/earth.obj"))
    {
        std::cout << "Failed to load earth.obj" << std::endl;
    }
    else
    {
        meshList.push_back(Earth);
    }
    Mesh *batmobile = new Mesh();
    if (!batmobile->CreateMeshFromOBJ("Models/Bat-Wing1989.obj"))
    {
        std::cout << "Failed to load Batmobile" << std::endl;
    }
    else
    {
        meshList.push_back(batmobile);
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

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data1 = stbi_load("Textures/texture.jpeg", &width1, &height1, &nrChannels1, 0);
    if (!data1)
    {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << "\n";
    }
    else
    {
        GLenum format = (nrChannels1 == 4) ? GL_RGBA : (nrChannels1 == 3) ? GL_RGB
                                                                          : GL_RED;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data1);
    }

    unsigned int texture2;
    int width2 = 0, height2 = 0, nrChannels2 = 0;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("Textures/2k_moon.jpeg", &width2, &height2, &nrChannels2, 0);
    if (!data2)
    {
        std::cerr << "Failed to load moon texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded moon texture: " << width2 << "x" << height2 << " Channels: " << nrChannels2 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format2 = (nrChannels2 == 4) ? GL_RGBA : (nrChannels2 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format2, width2, height2, 0, format2, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data2);
    }
    unsigned int texture3;
    int width3 = 0, height3 = 0, nrChannels3 = 0;
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data3 = stbi_load("Textures/BakedBatwing.png", &width3, &height3, &nrChannels3, 0);
    if (!data3)
    {
        std::cerr << "Failed to load Batmobile texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded Batmobile texture: " << width3 << "x" << height3 << " Channels: " << nrChannels3 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format3 = (nrChannels3 == 4) ? GL_RGBA : (nrChannels3 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format3, width3, height3, 0, format3, GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data3);
    }

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0].UseShader();
        GLuint uniformModel = shaderList[0].GetUniformLocation("model");
        GLuint uniformView = shaderList[0].GetUniformLocation("view");
        GLuint uniformProjection = shaderList[0].GetUniformLocation("projection");

        glm::mat4 view = camera.calculateViewMatrix();
        glm::vec3 cameraPos = camera.getCameraPosition();

        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, (GLfloat *)&lightPos);
        glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, (GLfloat *)&cameraPos);

        // -------------------------------
        // Render Duck
        // -------------------------------
        glm::mat4 modelDuck = glm::translate(glm::mat4(1.0f), glm::vec3(-1.3f, -0.20f, -1.5f));
        modelDuck = glm::scale(modelDuck, glm::vec3(0.07f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDuck));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1); // Duck texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[0]->RenderMesh(); // Duck

        // -------------------------------
        // Render earth
        // -------------------------------
        glm::mat4 modelEarth = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -5.0f));
        modelEarth = glm::scale(modelEarth, glm::vec3(3.8f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEarth));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2); // Earth texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[1]->RenderMesh(); // Earth

        
        // -------------------------------
        // Render Batmobile
        // -------------------------------
        glm::mat4 modelBatmobile = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f)); // วางด้านหน้า scene
       // modelBatmobile = glm::rotate(modelBatmobile, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelBatmobile = glm::scale(modelBatmobile, glm::vec3(0.5f));                             // ปรับขนาดให้สมจริง
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBatmobile));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3); // Batmobile texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);
        
        meshList[2]->RenderMesh(); // Batmobile

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
}