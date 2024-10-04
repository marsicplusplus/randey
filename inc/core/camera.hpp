#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
    public: 
        Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
        glm::mat4 getViewMatrix() const;

        void update(float df);
        glm::vec3 getPos() const {
            return mPos;
        }

    private:
        void updateVectors();    

        glm::vec3 mPos;
        glm::vec3 mFront;
        glm::vec3 mUp;
        glm::vec3 mRight;
        glm::vec3 mWorldUp;

        float mYaw = -90.0f;
        float mPitch = 0.0f;
};

#endif //__CAMERA_HPP__