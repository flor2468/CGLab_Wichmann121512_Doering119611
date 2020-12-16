#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include <iostream>
#include <cmath>
#include "Node.hpp"
#include "rgb.hpp"
#include <../../external/glm-0.9.6.3/glm/glm.hpp>

struct PointLightNode : public Node {
public:
    PointLightNode();
    PointLightNode(float const& lightIntensity_, RGB const& lightColor_, glm::fvec3 const& lightPosition_);

    void setLightIntensity(float const& lightIntensity);
    float getLightIntensity();
    void setLightColor(RGB const& lightColor);
    RGB getLightColor();
    void setLightPosition(glm::fvec3 lightPosition);
    glm::fvec3 getLightPosition();

private:
    float lightIntensity_ = {1.0f};
    RGB lightColor_ = {1.0f, 1.0f, 1.0f};
    glm::fvec3 lightPosition_ = {0.0f, 0.0f, 0.0f};

};

#endif 