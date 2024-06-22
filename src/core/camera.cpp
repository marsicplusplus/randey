#include "core/camera.hpp"

#include "managers/input_manager.hpp"
#include "GLFW/glfw3.h"


Camera::Camera(glm::vec3 pos, glm::vec3 up) : 
    mFront(glm::vec3(0.0, 0.0, -1.0)),
    mPos(pos),
    mWorldUp(up) {
    updateVectors();
}

glm::mat4  Camera::getViewMatrix() const {
    return glm::lookAt(mPos, mPos + mFront, mUp);
}

void Camera::update(float dt) {
    const float cameraSpeed = 1.5f * dt; // adjust accordingly
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_W))
        mPos += cameraSpeed * mFront;
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_S))
        mPos -= cameraSpeed * mFront;
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_Q))
        mPos += cameraSpeed * mUp;
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_E))
        mPos -= cameraSpeed * mUp;
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_A))
        mPos -= glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;
    if (InputManager::Instance()->isKeyDown(GLFW_KEY_D))
        mPos += glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));
    front.y = std::sin(glm::radians(mPitch));
    front.z = std::sin(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));

    mFront  = glm::normalize(front);

    mRight  = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp     = glm::normalize(glm::cross(mRight, mFront));
}