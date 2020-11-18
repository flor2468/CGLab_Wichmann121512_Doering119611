#include <SceneGraph.hpp>

SceneGraph::SceneGraph() {}
SceneGraph::SceneGraph(std::string const& name_, std::shared_ptr<Node> const& root_) : name_(name_), root_(root_) {}

std::string SceneGraph::getName() {
    return name_;
}


void SceneGraph::setName(std::string const& name) {
    name_ = name;
}

std::shared_ptr<Node> SceneGraph::getRoot() {
    return root_;
}


void SceneGraph::setRoot(std::shared_ptr<Node> const& root) {
    root_ = root;
}


std::string SceneGraph::printGraph() {
    
}


void SceneGraph::addNode(std::shared_ptr<GeometryNode> const& node) {
    nodes_.push_back(node);
}


std::list<std::shared_ptr<GeometryNode>> SceneGraph::getNodes() {
    return nodes_;
}