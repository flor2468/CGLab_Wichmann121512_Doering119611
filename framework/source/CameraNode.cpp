#include <CameraNode.hpp>

CameraNode::CameraNode() {}
CameraNode::CameraNode(bool isPerspective_, bool isEnabled_, glm::fmat4 const& projectionMatrix_) : isPerspective_(isPerspective_), isEnabled_(isEnabled_),
                       projectionMatrix_(projectionMatrix_) {}

bool CameraNode::getPerspective() {
    return isPerspective_;

}


bool CameraNode::getEnabled() {
    return isEnabled_;
}


void CameraNode::setEnabled(bool isEnabled) {
    isEnabled_ = isEnabled;
}


glm::fmat4 CameraNode::getProjectionMatrix() {
    return projectionMatrix_;
}


void CameraNode::setProjectionMatrix(glm::fmat4 const& projectionMatrix) {
    projectionMatrix_ = projectionMatrix;
}


