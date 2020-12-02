#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <iostream>
#include <cmath>
#include <Node.hpp> 

class GeometryNode : public Node {
public:
    GeometryNode();
    GeometryNode(model const& geometry_);
    GeometryNode(std::shared_ptr<Node> const& parent_, std::string const& name_);
    model getGeometry();
    void setGeometry(model const& geometry);

private:
    model geometry_;

};

#endif