#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <cmath>
#include <math.h>
#include <list>
#include <glm.hpp>
#include <mat4x4.hpp>
#include <iostream>
#include <memory>

class Node {
public:
    Node();
    Node(std::shared_ptr<Node> const& parent_, std::list<std::shared_ptr<Node>> const& children_, std::string const& name_, std::string const& path_, 
         int depth_, glm::dmat4x4 const& localtransform_, glm::dmat4x4 const& worldtransform_);

    std::shared_ptr<Node> getParent();
    void setParent(std::shared_ptr<Node> const& node);
    std::shared_ptr<Node> getChild(std::string const& child);
    std::list<std::shared_ptr<Node>> getChildrenList();
    void setChildren(std::list<std::shared_ptr<Node>> const& nodes);
    std::string getName();
    std::string getPath();
    int getDepth();
    glm::dmat4x4 getLocalTransform();
    void setLocalTransform(glm::dmat4x4 const& localtransform);
    glm::dmat4x4 getWorldTransform();
    void setWorldTransform(glm::dmat4x4 const& worldtransform);
    void addChildren(std::shared_ptr<Node> const& node);
    std::shared_ptr<Node> removeChildren(std::string const& children);

private:
    std::shared_ptr<Node> parent_;
    std::list<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    int depth_;
    glm::dmat4x4 localtransform_;
    glm::dmat4x4 worldtransform_;

};

#endif