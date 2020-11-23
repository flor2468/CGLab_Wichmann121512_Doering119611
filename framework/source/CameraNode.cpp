#include <CameraNode.hpp>

// constructors
CameraNode::CameraNode() {}
CameraNode::CameraNode(bool isPerspective_, bool isEnabled_, glm::fmat4 const& projectionMatrix_) : isPerspective_(isPerspective_), isEnabled_(isEnabled_),
                       projectionMatrix_(projectionMatrix_) {}

// getter for isPerspective_
bool CameraNode::getPerspective() {
    return isPerspective_;

}

// sgetter for isEnabled_
bool CameraNode::getEnabled() {
    return isEnabled_;
}

// setter for isEnabled_
void CameraNode::setEnabled(bool isEnabled) {
    isEnabled_ = isEnabled;
}

// getter for projectionMatrix_
glm::fmat4 CameraNode::getProjectionMatrix() {
    return projectionMatrix_;
}

// setter for projectionMatrix_
void CameraNode::setProjectionMatrix(glm::fmat4 const& projectionMatrix) {
    projectionMatrix_ = projectionMatrix;
}


