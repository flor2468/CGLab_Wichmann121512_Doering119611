#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <iostream>
#include <cmath>
// #include <external/glm-0.9.6.3/glm/mat4x4.hpp>
#include <Node.hpp>

class CameraNode : public Node {
public:
    CameraNode();
    CameraNode(bool isPerspective_, bool isEnabled_, glm::fmat4 const& projectionMatrix_);

    bool getPerspective();
    bool getEnabled();
    void setEnabled(bool isEnabled);
    glm::fmat4 getProjectionMatrix();
    void setProjectionMatrix(glm::fmat4 const& projectionMatrix);

private:
    bool isPerspective_;
    bool isEnabled_;
    glm::fmat4 projectionMatrix_;

};

#endif