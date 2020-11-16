#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <iostream>
#include <cmath>
#include <Node.hpp>

class SceneGraph {
public:
    SceneGraph();
    SceneGraph(std::string const& name_, Node const& root_);

    std::string getName();
    void setName(std::string const& name);
    Node getRoot();
    void setRoot(Node const& root);
    std::string printGraph();

private:
    std::string name_;
    Node root_;

};

#endif 