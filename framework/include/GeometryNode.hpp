#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <iostream>
#include <cmath>
#include <model.hpp>
#include "structs.hpp"
#include <Node.hpp> 

class GeometryNode : public Node {
public:
    GeometryNode();
    GeometryNode(model const& geometry_);
    GeometryNode(model_object const& meshObject_);
    GeometryNode(std::shared_ptr<Node> const& parent_, std::string const& name_);

    model getGeometry();
    void setGeometry(model const& geometry);

    model_object getMeshObject();
    void setMeshObject(model_object const& meshObject);

protected:
    model_object meshObject_;

private:
    model geometry_;

};

#endif