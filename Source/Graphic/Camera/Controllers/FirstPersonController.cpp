//
//  FirstPersonController.cpp
//  voxel-cone-tracing-mac
//
//  Created by Rafael Sabino on 11/5/17.
//  Copyright © 2017 Rafael Sabino. All rights reserved.
//

#include "Graphic/Camera/Controllers/FirstPersonController.h"


void FirstPersonController::update() {
    
    auto & app = Application::getInstance();
    
    double xpos, ypos;
    double xmid, ymid;
    GLFWwindow * window = Application::getInstance().currentWindow;

    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (firstUpdate) {
        targetCamera->forward = renderingCamera->forward;
        targetCamera->position = renderingCamera->position;
        firstUpdate = false;

        mouseXStart = xpos;
        mouseYStart = ypos;
    }
    
    // ----------
    // Rotation.
    // ----------

    float xDelta = mouseXStart - xpos;
    float yDelta = mouseYStart - ypos;
    
    float xRot = static_cast<float>(CAMERA_ROTATION_SPEED * xDelta);
    float yRot = static_cast<float>(CAMERA_ROTATION_SPEED * yDelta);
    
    // X rotation.

    targetCamera->forward = glm::rotateY(targetCamera->forward, xRot);
    
    // Y rotation.
    glm::vec3 newDirection = glm::rotate(targetCamera->forward, yRot, targetCamera->right());
    float a = glm::dot(newDirection, glm::vec3(0, 1, 0));
    if (abs(a) < 0.99)
        targetCamera->forward = newDirection;
    
    
    // ----------
    // Position.
    // ----------
    // Move forward.
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        targetCamera->position += targetCamera->front() * (float)FrameRate::deltaTime * CAMERA_SPEED;
    }
    // Move backward.
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        targetCamera->position -= targetCamera->front() * (float)FrameRate::deltaTime * CAMERA_SPEED;
    }
    // Strafe right.
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        targetCamera->position += targetCamera->right() * (float)FrameRate::deltaTime * CAMERA_SPEED;
    }
    // Strafe left.
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        targetCamera->position -= targetCamera->right() * (float)FrameRate::deltaTime * CAMERA_SPEED;
    }
    
    // Interpolate between target and current camera.
    auto * camera = renderingCamera;
    float rotationInterpolation= glm::clamp(FrameRate::deltaTime * CAMERA_ROTATION_INTERPOLATION_SPEED, 0.0, 1.0);
    float positionInterpolation = glm::clamp(FrameRate::deltaTime * CAMERA_POSITION_INTERPOLATION_SPEED, 0.0, 1.0);
    
    //printf("%f\n", rotationInterpolation);
    camera->forward = mix(camera->forward, targetCamera->front(), rotationInterpolation);
    camera->position = mix(camera->position, targetCamera->position, positionInterpolation);
    
    // Reset mouse position for next update iteration.
    //glfwSetCursorPos(window, xwidth / 2, yheight / 2);
    mouseXStart = xpos;
    mouseYStart = ypos;
    
    // Update view (camera) matrix.
    camera->updateViewMatrix();
}
