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

// light 1
glm::vec3 lightColour = glm::vec3(0.8f, 0.6f, 0.4f);
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
    Mesh *building1 = new Mesh();
    if (!building1->CreateMeshFromOBJ("Models/Gotham_Skyline_BuildingBlock05.obj"))
    {
        std::cout << "Failed to load Building" << std::endl;
    }
    else
    {
        meshList.push_back(building1);
    }
    Mesh *building2 = new Mesh();
    if (!building2->CreateMeshFromOBJ("Models/Gotham_Skyline_Building03.obj"))
    {
        std::cout << "Failed to load Building" << std::endl;
    }
    else
    {
        meshList.push_back(building2);
    }
    Mesh *building4 = new Mesh();
    if (!building4->CreateMeshFromOBJ("Models/Gotham_Skyline_BuildingBlock04.obj"))
    {
        std::cout << "Failed to load Building" << std::endl;
    }
    else
    {
        meshList.push_back(building4);
    }
    Mesh *building5 = new Mesh();
    if (!building5->CreateMeshFromOBJ("Models/Gotham_Skyline_Building03.obj"))
    {
        std::cout << "Failed to load Building" << std::endl;
    }
    else
    {
        meshList.push_back(building5);
    }
    Mesh *building6 = new Mesh();
    if (!building6->CreateMeshFromOBJ("Models/Gotham_Skyline_BuildingBlock07.obj"))
    {
        std::cout << "Failed to load Building" << std::endl;
    }
    else
    {
        meshList.push_back(building6);
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
    unsigned int texture4;
    int width4 = 0, height4 = 0, nrChannels4 = 0;
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data4 = stbi_load("Textures/Gotham_Skyline_BuildingBLock01_D.png", &width4, &height4, &nrChannels4, 0);
    if (!data4)
    {
        std::cerr << "Failed to load building texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded building texture: " << width4 << "x" << height4 << " Channels: " << nrChannels4 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format4 = (nrChannels4 == 4) ? GL_RGBA : (nrChannels4 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format4, width4, height4, 0, format4, GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data4);
    }
    unsigned int texture5;
    int width5 = 0, height5 = 0, nrChannels5 = 0;
    glGenTextures(1, &texture5);
    glBindTexture(GL_TEXTURE_2D, texture5);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data5 = stbi_load("Textures/Gotham_Skyline_Building03_D1.jpg", &width5, &height5, &nrChannels5, 0);
    if (!data5)
    {
        std::cerr << "Failed to load building texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded building texture: " << width5 << "x" << height5 << " Channels: " << nrChannels5 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format5 = (nrChannels5 == 4) ? GL_RGBA : (nrChannels5 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format5, width5, height5, 0, format5, GL_UNSIGNED_BYTE, data5);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data5);
    }
    unsigned int texture7;
    int width7 = 0, height7 = 0, nrChannels7 = 0;
    glGenTextures(1, &texture7);
    glBindTexture(GL_TEXTURE_2D, texture7);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data7 = stbi_load("Textures/Gotham_Skyline_BuildingBLock01_D.png", &width7, &height7, &nrChannels7, 0);
    if (!data7)
    {
        std::cerr << "Failed to load building texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded building texture: " << width7 << "x" << height7 << " Channels: " << nrChannels7 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format7 = (nrChannels7 == 4) ? GL_RGBA : (nrChannels7 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format7, width7, height7, 0, format7, GL_UNSIGNED_BYTE, data7);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data7);
    }
    unsigned int texture8;
    int width8 = 0, height8 = 0, nrChannels8 = 0;
    glGenTextures(1, &texture8);
    glBindTexture(GL_TEXTURE_2D, texture8);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data8 = stbi_load("Textures/Gotham_Skyline_Building03_D1.jpg", &width8, &height8, &nrChannels8, 0);
    if (!data8)
    {
        std::cerr << "Failed to load building texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded building texture: " << width8 << "x" << height8 << " Channels: " << nrChannels8 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format8 = (nrChannels8 == 4) ? GL_RGBA : (nrChannels8 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format8, width8, height8, 0, format8, GL_UNSIGNED_BYTE, data8);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data8);
    }
    unsigned int texture9;
    int width9 = 0, height9 = 0, nrChannels9 = 0;
    glGenTextures(1, &texture9);
    glBindTexture(GL_TEXTURE_2D, texture9);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data9 = stbi_load("Textures/Gotham_Skyline_BuildingBLock01_D.png", &width9, &height9, &nrChannels9, 0);
    if (!data9)
    {
        std::cerr << "Failed to load building texture! reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }
    else
    {
        std::cout << "Loaded building texture: " << width9 << "x" << height9 << " Channels: " << nrChannels9 << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum format9 = (nrChannels9 == 4) ? GL_RGBA : (nrChannels9 == 3) ? GL_RGB
                                                                           : GL_RED;
        glTexImage2D(GL_TEXTURE_2D, 0, format9, width9, height9, 0, format9, GL_UNSIGNED_BYTE, data9);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data9);
    }

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

        // light
        glUniform3fv(shaderList[0].GetUniformLocation("lightColour"), 1, glm::value_ptr(lightColour));
        glUniform3fv(shaderList[0].GetUniformLocation("lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(shaderList[0].GetUniformLocation("viewPos"), 1, glm::value_ptr(cameraPos));


        // -------------------------------
        // Render Duck
        // -------------------------------
        glm::mat4 modelDuck = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        modelDuck = glm::rotate(modelDuck, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // modelDuck = glm::rotate(modelDuck, glm::radians(.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelDuck = glm::scale(modelDuck, glm::vec3(0.05f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDuck));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1); // Duck texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[0]->RenderMesh(); // Duck

        // -------------------------------
        // Render earth
        // -------------------------------
        glm::mat4 modelEarth = glm::translate(glm::mat4(1.0f), glm::vec3(-47.0f, 25.0f, -60.0f));
        modelEarth = glm::scale(modelEarth, glm::vec3(7.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelEarth));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2); // Earth texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[1]->RenderMesh(); // Earth

        // -------------------------------
        // Render Batmobile
        // -------------------------------
        glm::mat4 modelBatmobile = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -20.0f, -6.0f)); // วางด้านหน้า scene
        modelBatmobile = glm::rotate(modelBatmobile, glm::radians(38.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelBatmobile = glm::scale(modelBatmobile, glm::vec3(5.0f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBatmobile));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3); // Batmobile texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[2]->RenderMesh(); // Batmobile

        // -------------------------------
        // Render building1
        // -------------------------------
        glm::mat4 modelbuilding1 = glm::translate(glm::mat4(1.0f), glm::vec3(32.0f, -40.0f, -10.0f)); // วางด้านหน้า scene
        modelbuilding1 = glm::rotate(modelbuilding1, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelbuilding1 = glm::scale(modelbuilding1, glm::vec3(0.0020f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelbuilding1));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4); // building1 texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[3]->RenderMesh(); // buliding1

        // -------------------------------
        // Render building2
        // -------------------------------
        glm::mat4 modelbuilding2 = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -40.0f, -25.0f)); // วางด้านหน้า scene
        modelbuilding2 = glm::rotate(modelbuilding2, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelbuilding2 = glm::scale(modelbuilding2, glm::vec3(0.0015f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelbuilding2));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture5); // building2 texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[4]->RenderMesh(); // buliding2


        // -------------------------------
        // Render building4
        // -------------------------------
        glm::mat4 modelbuilding4 = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, -40.0f, 20.0f)); // วางด้านหน้า scene
        modelbuilding4 = glm::rotate(modelbuilding4, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelbuilding4 = glm::scale(modelbuilding4, glm::vec3(0.0020f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelbuilding4));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture7); // building4 texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[5]->RenderMesh(); // buliding4

        // -------------------------------
        // Render building5
        // -------------------------------
        glm::mat4 modelbuilding5 = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, -40.0f, 7.0f)); // วางด้านหน้า scene
        modelbuilding5 = glm::rotate(modelbuilding5, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelbuilding5 = glm::scale(modelbuilding5, glm::vec3(0.0015f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelbuilding5));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture8); // building5 texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[6]->RenderMesh(); // buliding5

        // -------------------------------
        // Render building6
        // -------------------------------
        glm::mat4 modelbuilding6 = glm::translate(glm::mat4(1.0f), glm::vec3(-25.0f, -40.0f, -35.0f)); // วางด้านหน้า scene
        modelbuilding6 = glm::rotate(modelbuilding6, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelbuilding6 = glm::scale(modelbuilding6, glm::vec3(0.0025f)); // ปรับขนาดให้สมจริง

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelbuilding6));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture9); // building6 texture
        glUniform1i(shaderList[0].GetUniformLocation("texture1"), 0);

        meshList[7]->RenderMesh(); // buliding6

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
}
