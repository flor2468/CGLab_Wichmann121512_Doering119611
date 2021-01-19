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
         int depth_, glm::fmat4 const& localtransform_, glm::fmat4 const& worldtransform_, float const& speed_, glm::vec3 const& size_, 
         glm::fvec3 const& position_, glm::fmat4 const& rotMat_);

    Node(std::shared_ptr<Node> const& parent_, std::string const& name_, glm::fvec3 const& color_ = {1.0f, 1.0f, 1.0f});

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

    float getSpeed();
    void setSpeed(float const& speed);
    glm::fvec3 getSize();
    void setSize(glm::fvec3 const& size);
    glm::fvec3 getPosition();
    void setPosition(glm::fvec3 const& position);
    glm::fmat4 getRotMat();
    void setRotMat(glm::fmat4 const& rotMat);
    void setColor(glm::vec3 const& color);
    glm::fvec3 getColor();
    char* getTextureImage();
    void setTextureImage(char* const& textureImage);

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
    float speed_ {0.0f};
    glm::fvec3 size_ {0.0f, 0.0f, 0.0f};
    glm::fvec3 position_ {0.0f, 0.0f, 0.0f};
    glm::fmat4 rotMat_ {glm::fmat4{}};

    glm::fvec3 color_;
    char* textureImage_;

};

#endif