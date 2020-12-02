#include <Node.hpp>

// constructors
Node::Node() {}
Node::Node(std::shared_ptr<Node> const& parent_, std::list<std::shared_ptr<Node>> const& children_, std::string const& name_, std::string const& path_, 
           int depth_, glm::fmat4 const& localtransform_, glm::fmat4 const& worldtransform_) : parent_(nullptr), children_(),
           name_(""), path_(""), depth_(0), localtransform_(localtransform_), worldtransform_(worldtransform_) {}
        
Node::Node(std::shared_ptr<Node> const& parent_, std::string const& name_) : parent_(nullptr), name_("") {}

// getter for parent_
std::shared_ptr<Node> Node::getParent() {
    return parent_;
}

// setter for parent_
void Node::setParent(std::shared_ptr<Node> const& node) {
    parent_ = node;
}

// getter for a child-node
std::shared_ptr<Node> Node::getChild(std::string const& child) {
    for (std::shared_ptr<Node> n : children_) {
        if (n->getName() == child) {
            return n;
        }
    }
}

// getter for the list of child-nodes
std::list<std::shared_ptr<Node>> Node::getChildrenList() {
    return children_;
}

// setter for the list of child-nodes
void Node::setChildren(std::list<std::shared_ptr<Node>> const& nodes) {
    for (std::shared_ptr<Node> c : children_) {
        for (std::shared_ptr<Node> n : nodes) {
            c = n;
        }
    }
}

// getter for the name_
std::string Node::getName() {
    return name_;
}

// getter for the path_
std::string Node::getPath() {
    return path_;
}

// getter for the depth_
int Node::getDepth() {
    return depth_;
}

// getter for the localtransform_
glm::fmat4 Node::getLocalTransform() {
    return localtransform_;
}

// setter for the localtransform_
void Node::setLocalTransform(glm::fmat4 const& localtransform) {
    localtransform_ = localtransform;
}

// getter for the worldtransform_
glm::fmat4 Node::getWorldTransform() {
    return worldtransform_;
}

// setter for the worldtransform_
void Node::setWorldTransform(glm::fmat4 const& worldtransform) {
    worldtransform_ = worldtransform;
}

// setter for the name_
void Node::setName(std::string const& name) {
    name_ = name;
}

// function for adding a child to the "children_"-list
void Node::addChildren(std::shared_ptr<Node> const& node) {
    children_.push_back(node);
}

// function for removing a child from the "children_"-list
std::shared_ptr<Node> Node::removeChildren(std::string const& child) {
    for (std::shared_ptr<Node> c : children_) {
        if (c->getName().compare(child) == 0) {
            children_.remove(c);
        }
    }
}

// getter for meshObject_
model_object Node::getMeshObject() {
    return meshObject_;
}

// setter for meshObject_
void Node::setMeshObject(model_object const& meshObject) {
    meshObject_ = meshObject;
}
