#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <cmath>
#include <math.h>
#include <list>
#include <../../external/glm-0.9.6.3/glm/glm.hpp>
#include <../../external/glm-0.9.6.3/glm/mat4x4.hpp>
#include <iostream>
#include <memory>
#include <model.hpp>
#include "structs.hpp"

class Node {
public:
    Node();
    Node(std::shared_ptr<Node> const& parent_, std::list<std::shared_ptr<Node>> const& children_, std::string const& name_, std::string const& path_, 
         int depth_, glm::fmat4 const& localtransform_, glm::fmat4 const& worldtransform_);

    Node(std::shared_ptr<Node> const& parent_, std::string const& name_);

    std::shared_ptr<Node> getParent();
    void setParent(std::shared_ptr<Node> const& node);
    std::shared_ptr<Node> getChild(std::string const& child);
    std::list<std::shared_ptr<Node>> getChildrenList();
    void setChildren(std::list<std::shared_ptr<Node>> const& nodes);
    void setName(std::string const& name);
    std::string getName();
    std::string getPath();
    int getDepth();
    glm::fmat4 getLocalTransform();
    void setLocalTransform(glm::fmat4 const& localtransform);
    glm::fmat4 getWorldTransform();
    void setWorldTransform(glm::fmat4 const& worldtransform);
    void addChildren(std::shared_ptr<Node> const& node);
    std::shared_ptr<Node> removeChildren(std::string const& children);
    model getGeometry();
    void setGeometry(model const& geometry);
    model_object getMeshObject();
    void setMeshObject(model_object const& meshObject);

protected:
    model_object meshObject_;

private:
    std::shared_ptr<Node> parent_;
    std::list<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    int depth_;
    glm::fmat4 localtransform_;
    glm::fmat4 worldtransform_;

};

#endif