#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gps {
    
    enum MOVE_DIRECTION {MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT};
    
    class Camera {

    public:
        //Camera constructor
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction, float speed);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void rotate(float pitch, float yaw);
        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch);
        void changeCrouchState();
        glm::vec3 getCameraPosition();
        void setCameraPosition(glm::vec3 cameraPosition);
        void setCameraTarget(glm::vec3 cameraTarget);
        void changeToCameraView();
        void changeSensitivity(float sensitivity);
        void changeToNormalView();
        glm::vec3 getCameraTarget();
        void changeSecurityCameraMode();
        float getPitch();
        float getYaw();
        
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        bool doBump;
        int bumpCounter;
        int maxBumpCounter;
        float sensitivity;
        float yaw;
        float pitch;
        bool crouched;
        float oldYaw;
        float oldPitch;
        bool securityCameraMode;
        float minXPos;
        float maxXPos;
        float minZPos;
        float maxZPos;

    };    
}

#endif /* Camera_hpp */
