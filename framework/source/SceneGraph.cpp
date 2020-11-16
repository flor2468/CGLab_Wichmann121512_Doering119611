#include <SceneGraph.hpp>

SceneGraph::SceneGraph() {}
SceneGraph::SceneGraph(std::string const& name_, Node const& root_) : name_(name_), root_(root_) {}

std::string SceneGraph::getName() {
    return name_;
}


void SceneGraph::setName(std::string const& name) {
    name_ = name;
}

Node SceneGraph::getRoot() {
    return root_;
}


void SceneGraph::setRoot(Node const& root) {
    root_ = root;
}


std::string SceneGraph::printGraph() {
    
}



