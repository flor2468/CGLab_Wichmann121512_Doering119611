#ifndef STAR_HPP
#define STAR_HPP

#include <iostream>
#include <cmath>
#include <math.h>
#include <list>
#include <../../external/glm-0.9.6.3/glm/glm.hpp>

struct Star {
    std::vector<float> position_;
    std::vector<float> color_; 
    std::vector<float> posColData_;
    model_object model_;

};


#endif