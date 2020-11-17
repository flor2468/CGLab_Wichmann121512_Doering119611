#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <iostream>
// #include <stdio.h>
#include <cmath>
#include <memory> 
#include <Node.hpp>

class SceneGraph {
public:
    SceneGraph();
    SceneGraph(std::string const& name_, std::shared_ptr<Node> const& root_);

    std::string getName();
    std::shared_ptr<Node> getRoot();
    std::string printGraph();

private:
    void setName(std::string const& name);
    void setRoot(std::shared_ptr<Node> const& root);

    std::string name_;
    std::shared_ptr<Node> root_;

};

#endif 