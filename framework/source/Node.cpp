#include <Node.hpp>

Node::Node() {}
Node::Node(std::shared_ptr<Node> const& parent_, std::list<std::shared_ptr<Node>> const& children_, std::string const& name_, std::string const& path_, 
           int depth_, glm::dmat4x4 const& localtransform_, glm::dmat4x4 const& worldtransform_) : parent_(parent_), children_(children_),
           name_(name_), path_(path_), depth_(depth_), localtransform_(localtransform_), worldtransform_(worldtransform_) {}

std::shared_ptr<Node> Node::getParent() {
    return parent_;
}


void Node::setParent(std::shared_ptr<Node> const& node) {
    parent_ = node;
}


std::shared_ptr<Node> Node::getChild(std::string const& child) {
    for (std::shared_ptr<Node> n : children_) {
        if (n->getName() == child) {
            return n;
        }
    }
}

std::list<std::shared_ptr<Node>> Node::getChildrenList() {
    return children_;
}


void Node::setChildren(std::list<std::shared_ptr<Node>> const& nodes) {
    for (std::shared_ptr<Node> c : children_) {
        for (std::shared_ptr<Node> n : nodes) {
            c = n;
        }
    }
}


std::string Node::getName() {
    return name_;
}


std::string Node::getPath() {
    return path_;
}


int Node::getDepth() {
    return depth_;
}


glm::dmat4x4 Node::getLocalTransform() {
    return localtransform_;
}


void Node::setLocalTransform(glm::dmat4x4 const& localtransform) {
    localtransform_ = localtransform;
}


glm::dmat4x4 Node::getWorldTransform() {
    return worldtransform_;
}


void Node::setWorldTransform(glm::dmat4x4 const& worldtransform) {
    worldtransform_ = worldtransform;
}


void Node::addChildren(std::shared_ptr<Node> const& node) {
    children_.push_back(node);
}


std::shared_ptr<Node> Node::removeChildren(std::string const& child) {
    for (std::shared_ptr<Node> c : children_) {
        if (c->getName().compare(child) == 0) {
            children_.remove(c);
        }
    }
}
