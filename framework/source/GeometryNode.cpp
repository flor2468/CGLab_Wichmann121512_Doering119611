#include <GeometryNode.hpp>

GeometryNode::GeometryNode() {}
GeometryNode::GeometryNode(model const& geometry_) : geometry_(geometry_) {}

model const& GeometryNode::getGeometry() {
    return geometry_;
}


void GeometryNode::setGeometry(model const& geometry) {
    geometry_ = geometry;
}

