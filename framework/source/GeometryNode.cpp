#include <GeometryNode.hpp>

GeometryNode::GeometryNode() {}
GeometryNode::GeometryNode(model const& geometry_) : geometry_(geometry_) {}

GeometryNode::GeometryNode(std::shared_ptr<Node> const& parent_, std::string const& name_) {
    Node(parent_, name_);
}

model const& GeometryNode::getGeometry() {
    return geometry_;
}


void GeometryNode::setGeometry(model const& geometry) {
    geometry_ = geometry;
}

