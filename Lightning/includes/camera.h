#ifndef CAMERA_H
#define CAMERA_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_movement{
    FOWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//default camera vaues
const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {

public:
    //相机参数
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //欧拉角
    float yaw;
    float pitch;

    //相机选项
    float movementSpeed;
    float mouseSensiitivity;
    float zoom;

    //用向量初始化
    Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.0f, 0.f), float yaw = YAW, float pitch = PITCH)
    : position(pos), front(glm::vec3(0.f,0.f,-1.f) ), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(SPEED), mouseSensiitivity(SENSITIVITY), zoom(ZOOM)
    {
        updateCameraVectors();
    }

    //用标量初始化
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
    : position(glm::vec3(posX, posY, posZ)), front(glm::vec3(0.f,0.f,-1.f)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch)
    {
        updateCameraVectors();
    }

    //计算view矩阵
    glm::mat4 getViewMatrix() {
        return glm::lookAt(
            position,           //cmera position
            position + front,   //target position, 相机到目标的向量的相反向量
            up                  // up direction
        );
    }
    
    //处理键盘输入
    void processKeyboard(Camera_movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == FOWARD) {
            position += front * velocity;
        }
        if (direction == BACKWARD) {
            position -= front * velocity;
        }
        if (direction == RIGHT) {
            position += right * velocity;
        }
        if (direction == LEFT) {
            position -= right * velocity;
        }
    }

    //处理鼠标输入
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = true) {
        x_offset *= mouseSensiitivity;
        y_offset *= mouseSensiitivity;
        yaw += x_offset;
        pitch += y_offset;

        //保证pitch超过限制，屏幕不会翻转
        if (constrainPitch == true) {
            if (pitch > 89.f) {
                pitch == 89.f;

            }
            if (pitch < -89.f){
                pitch = -89.f;
            }
        }
        updateCameraVectors();

    }
    //处理鼠标滚轮
    void processMouseScoll(float y_offset) {
        zoom -= (float)y_offset;
        if (zoom < 1.f) {
            zoom = 1.f;
        }
        if (zoom > 45.f) {
            zoom = 45.f;
        }
    }

private:
    private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors(){
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif