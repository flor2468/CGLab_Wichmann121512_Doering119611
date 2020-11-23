#include <SceneGraph.hpp>

// constructors
SceneGraph::SceneGraph() {}
SceneGraph::SceneGraph(std::string const& name_, std::shared_ptr<Node> const& root_) : name_(name_), root_(root_) {}

// getter for name_
std::string SceneGraph::getName() {
    return name_;
}

// setter for name_
void SceneGraph::setName(std::string const& name) {
    name_ = name;
}

// getter for root_
std::shared_ptr<Node> SceneGraph::getRoot() {
    return root_;
}

// setter for root_
void SceneGraph::setRoot(std::shared_ptr<Node> const& root) {
    root_ = root;
}

// function for printing the graph
std::string SceneGraph::printGraph() {
    
}

// function for adding a node to the "nodes_"-list
void SceneGraph::addNode(std::shared_ptr<GeometryNode> const& node) {
    nodes_.push_back(node);
}

// getter for the nodes_
std::list<std::shared_ptr<GeometryNode>> SceneGraph::getNodes() {
    return nodes_;
}