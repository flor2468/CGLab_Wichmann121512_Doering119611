#include "PointLightNode.hpp"

PointLightNode::PointLightNode() {}
PointLightNode::PointLightNode(float const& lightIntensity_, RGB const& lightColor_, glm::fvec3 const& lightPosition_) : 
                                lightIntensity_(1.0f), lightColor_({1.0f, 1.0f, 1.0f}), lightPosition_({0.0f, 0.0f, 0.0f}) {}

void PointLightNode::setLightIntensity(float const& lightIntensity) {
    lightIntensity_ = lightIntensity;
}

float PointLightNode::getLightIntensity() {
    return lightIntensity_;
}

void PointLightNode::setLightColor(RGB const& lightColor) {
    lightColor_ = lightColor;
}

RGB PointLightNode::getLightColor() {
    return lightColor_;
}

void PointLightNode::setLightPosition(glm::fvec3 lightPosition) {
    lightPosition_ = lightPosition;
}

glm::fvec3 PointLightNode::getLightPosition() {
    return lightPosition_;
}
