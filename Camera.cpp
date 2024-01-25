#include "Camera.hpp"
#include <time.h>
#include <iostream>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));
        this->cameraUpDirection = cameraUp;
        this->doBump = false;
        this->bumpCounter = 0;
       // this->maxBumpCounter = 26; ecran mare
        this->maxBumpCounter = 52;
        this->sensitivity = 0.2f;
        this->yaw = -90.0f;
        this->pitch = 0.0f;
        this->crouched = false;
        this->securityCameraMode = false;
        this->minXPos=-30.0f;
        this->maxXPos=20.0f;
        this->minZPos=-30.0f;
        this->maxZPos=30.0f;

        
        srand(time(NULL));
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
        //return glm::mat4();
    }

    void Camera::changeCrouchState() {
        this->cameraPosition.y = this->crouched ? this->cameraPosition.y + 0.5f : this->cameraPosition.y - 0.5f;
        crouched = !crouched;
    }

    glm::vec3 Camera::getCameraPosition() {
		return this->cameraPosition;
	}
    glm::vec3 Camera::getCameraTarget() {
        return this->cameraTarget;
    }

    void Camera::setCameraPosition(glm::vec3 cameraPosition) {
		this->cameraPosition = cameraPosition;
	}

    void Camera::setCameraTarget(glm::vec3 cameraTarget) {
        this->cameraTarget = cameraTarget;
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        bumpCounter++;
        if (bumpCounter <= maxBumpCounter / 2)
        {
            cameraPosition.y += 0.015;
            doBump = false;
        }
        else
        {
            cameraPosition.y -= 0.015;
            doBump = true;
        }
        if (bumpCounter == maxBumpCounter)
        {
            //maxBumpCounter = 24 + rand() % 6; ecran mare
            maxBumpCounter = 48 + rand() % 12;
            if (maxBumpCounter % 2 == 1)
            {
                maxBumpCounter++;
            }
            bumpCounter = 0;
        }

        glm::vec3 movement = glm::vec3(0.0f);

        switch (direction)
        {
        case gps::MOVE_FORWARD:
            movement = cameraFrontDirection * speed;
            break;
        case gps::MOVE_BACKWARD:
            movement = -cameraFrontDirection * speed;
            break;
        case gps::MOVE_RIGHT:
            movement = cameraRightDirection * speed;
            break;
        case gps::MOVE_LEFT:
            movement = -cameraRightDirection * speed;
            break;
        }

        // Limit movement to the x-z plane
        movement.y = 0.0f;

        cameraPosition += movement;
        if(cameraPosition.x < minXPos)
			cameraPosition.x = minXPos;
        if(cameraPosition.x > maxXPos)
            cameraPosition.x = maxXPos;
        if(cameraPosition.z < minZPos)
            cameraPosition.z = minZPos;
        if(cameraPosition.z > maxZPos)
            cameraPosition.z = maxZPos;
    }

    void Camera::changeToCameraView()
    {
        cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        this->oldPitch = this->pitch;
        this->oldYaw = this->yaw;
        this->yaw = 120.0f;
        this->pitch = -10.0f;
        rotate(pitch, yaw);
    }

    void Camera::changeToNormalView()
    {
        cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        this->yaw = this->oldYaw;
        this->pitch = this->oldPitch;
        rotate(pitch, yaw);
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(front);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    void Camera::changeSensitivity(float sensitivity) {
		this->sensitivity = sensitivity;
	}

    void Camera::changeSecurityCameraMode() {
		this->securityCameraMode = !this->securityCameraMode;
	}

    float Camera::getPitch() {
		return this->pitch;
	}

    float Camera::getYaw() {
        return this->yaw;
    }

    void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        // Constrain pitch to avoid camera flipping
        if (constrainPitch) {
            if (!securityCameraMode)
            {
                if (pitch > 89.0f) pitch = 89.0f;
                if (pitch < -89.0f) pitch = -89.0f;
            }
            else
            {
                std::cout<<yaw<<std::endl;
				if (yaw > 151.57f) yaw = 151.57f;
				if (yaw < 118.92f) yaw = 118.92f;
			}
        }

        // Update camera orientation
        rotate(pitch, yaw);
    }
}