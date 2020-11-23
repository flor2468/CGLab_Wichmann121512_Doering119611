#include <GeometryNode.hpp>

// constructors
GeometryNode::GeometryNode() {}
GeometryNode::GeometryNode(model const& geometry_) : geometry_(geometry_) {}
GeometryNode::GeometryNode(model_object const& meshObject_) : meshObject_(meshObject_) {}
GeometryNode::GeometryNode(std::shared_ptr<Node> const& parent_, std::string const& name_) {
    Node(parent_, name_);
}

// getter for geometry_
model GeometryNode::getGeometry() {
    return geometry_;
}

// setter for geometry_
void GeometryNode::setGeometry(model const& geometry) {
    geometry_ = geometry;
}

// getter for meshObject_
model_object GeometryNode::getMeshObject() {
    return meshObject_;
}

// setter for meshObject_
void GeometryNode::setMeshObject(model_object const& meshObject) {
    meshObject_ = meshObject;
}