#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include <iostream>
#include <cmath>
#include "Node.hpp"
#include "rgb.hpp"
#include <../../external/glm-0.9.6.3/glm/glm.hpp>

struct PointLightNode : public Node {
public:
    // PointLightNode();

private:
    float lightIntensity_;
    RGB lightColor_;

};

#endif 