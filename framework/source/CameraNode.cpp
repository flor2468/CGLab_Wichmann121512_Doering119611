#include <CameraNode.hpp>

CameraNode::CameraNode() {}
CameraNode::CameraNode(bool isPerspective_, bool isEnabled_, glm::dmat4x4 const& projectionMatrix_) : isPerspective_(isPerspective_), isEnabled_(isEnabled_),
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


glm::dmat4x4 CameraNode::getProjectionMatrix() {
    return projectionMatrix_;
}


void CameraNode::setProjectionMatrix(glm::dmat4x4 const& projectionMatrix) {
    projectionMatrix_ = projectionMatrix;
}


