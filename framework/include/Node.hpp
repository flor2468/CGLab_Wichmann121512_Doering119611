#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <cmath>
#include <math.h>
#include <list>
#include <glm.hpp>
#include <mat4x4.hpp>

class Node {
public:
    Node();
    Node(Node const& parent_, std::list<Node> const& children_, std::string const& name_, std::string const& path_, 
         int depth_, glm::dmat4x4 const& localtransform_, glm::dmat4x4 const& worldtransform_);

    Node getParent();
    void setParent(Node const& node);
    Node getChild(std::string const& child);
    std::list<Node> getChildrenList();
    void setChildren(std::list<Node> const& nodes);
    std::string getName();
    std::string getPath();
    int getDepth();
    glm::dmat4x4 getLocalTransform();
    void setLocalTransform(glm::dmat4x4 const& localtransform);
    glm::dmat4x4 getWorldTransform();
    void setWorldTransform(glm::dmat4x4 const& worldtransform);
    void addChildren(Node const& node);
    Node removeChildren(std::string const& children);

private:
    Node const& parent_;
    std::list<Node> const& children_;
    std::string const& name_;
    std::string const& path_;
    int depth_;
    glm::dmat4x4 const& localtransform_;
    glm::dmat4x4 const& worldtransform_;

};

#endif