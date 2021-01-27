#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <iostream>
// #include <stdio.h>
#include <cmath>
#include <memory> 
#include <Node.hpp>
#include "GeometryNode.hpp"
#include "PointLightNode.hpp"
#include "CameraNode.hpp"

class SceneGraph {
public:
    SceneGraph();
    SceneGraph(std::string const& name_, std::shared_ptr<Node> const& root_);

    std::string getName();
    std::shared_ptr<Node> getRoot();
    std::string printGraph();
    
    void addNode(std::shared_ptr<Node> const& node);
    std::list<std::shared_ptr<Node>> getNodes();

    void addLightNode(std::shared_ptr<PointLightNode> const& lightNode);
    std::list<std::shared_ptr<PointLightNode>> getLightNodes();

    std::shared_ptr<CameraNode> getCamera();
    void setCamera(std::shared_ptr<CameraNode> camera);

    std::list<std::pair<int, texture_object>> getTextureObjects();
    void addTextureObjects(std::pair<int, texture_object> textureObject);
    texture_object getSingleTextureObject(int index);

    std::list<std::pair<int, texture_object>> getNormalTextureObjects();
    void addNormalTextureObjects(std::pair<int, texture_object> textureObject);
    texture_object getSingleNormalTextureObject(int index);

    void setSize(int size);
    int getSize();

protected:
    std::list<std::shared_ptr<Node>> nodes_;
    std::list<std::shared_ptr<PointLightNode>> lightNodes_;
    std::list<std::pair<int, texture_object>> textureObjects_;
    std::list<std::pair<int, texture_object>> normalTextureObjects_;

private:
    void setName(std::string const& name);
    void setRoot(std::shared_ptr<Node> const& root);

    std::string name_;
    std::shared_ptr<Node> root_;
    std::shared_ptr<CameraNode> camera_;
    int size_;

};

#endif 