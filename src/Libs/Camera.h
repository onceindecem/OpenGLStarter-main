#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    {
        position = startPosition;
        worldUp = startUp;
        yaw = startYaw;
        pitch = startPitch;
        moveSpeed = startMoveSpeed;
        turnSpeed = startTurnSpeed;
        update();
    }

    void keyControl(bool* keys, GLfloat deltaTime)
    {
        GLfloat velocity = moveSpeed * deltaTime;

        if (keys[GLFW_KEY_W])
            position += front * velocity;
        if (keys[GLFW_KEY_S])
            position -= front * velocity;
        if (keys[GLFW_KEY_A])
            position -= right * velocity;
        if (keys[GLFW_KEY_D])
            position += right * velocity;
    }

    void mouseControl(GLfloat xChange, GLfloat yChange)
    {
        xChange *= turnSpeed;
        yChange *= turnSpeed;

        yaw += xChange;
        pitch += yChange;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        update();
    }

    glm::mat4 calculateViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    glm::vec3 getCameraPosition() { return position; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    GLfloat yaw, pitch;
    GLfloat moveSpeed, turnSpeed;

    void update()
    {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
