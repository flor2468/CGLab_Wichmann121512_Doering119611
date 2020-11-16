#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <iostream>
#include <cmath>
#include <model.hpp>
#include <Node.hpp> 

class GeometryNode : public Node {
public:
    GeometryNode();
    GeometryNode(model const& geometry_);
    model const& getGeometry();
    void setGeometry(model const& geometry);

private:
    model const& geometry_;

};

#endif