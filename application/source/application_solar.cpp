#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <glm/gtx/string_cast.hpp>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 // sets the distance of the planet, the bigger the (z) value, the greater the distance
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 30.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::makeSolarSystem() {

  // copied from initializeGeometry() to load another sphere
  // model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // root

  Node root = Node(nullptr, "root");
  std::shared_ptr<Node> rootPointer = std::make_shared<Node>(root);

  // SceneGraph

  planetGraph_ = SceneGraph("scene1", rootPointer);

  // camera

  CameraNode camera = CameraNode();

  // light

  PointLightNode light = PointLightNode();
  std::shared_ptr<PointLightNode> lightPointer = std::make_shared<PointLightNode>(light);

  root.addChildren(lightPointer);

  // sun
  
  GeometryNode sun = GeometryNode(rootPointer, "sun");
  std::shared_ptr<GeometryNode> sunPointer = std::make_shared<GeometryNode>(sun);
  // sunPointer->setGeometry(planet_model);
  sunPointer->setMeshObject(planet_object);
  sunPointer->setName("sun");
  light.addChildren(sunPointer);
  light.setParent(rootPointer);
  sun.setParent(lightPointer);

  // mercury

  Node mercury = Node(rootPointer, "mercury");
  std::shared_ptr<Node> mercuryPointer = std::make_shared<Node>(mercury);

  GeometryNode geometryMercury = GeometryNode(mercuryPointer, "geometryMercury");
  std::shared_ptr<GeometryNode> geometryMercuryPointer = std::make_shared<GeometryNode>(geometryMercury);

  geometryMercuryPointer->setMeshObject(planet_object);
  geometryMercuryPointer->setName("geometryMercury");
  root.addChildren(mercuryPointer);
  mercury.addChildren(geometryMercuryPointer);
  mercury.setParent(rootPointer);
  geometryMercury.setParent(mercuryPointer);


  // venus

  Node venus = Node(rootPointer, "venus");
  std::shared_ptr<Node> venusPointer = std::make_shared<Node>(venus);

  GeometryNode geometryVenus = GeometryNode(venusPointer, "geometryVenus");
  std::shared_ptr<GeometryNode> geometryVenusPointer = std::make_shared<GeometryNode>(geometryVenus);

  geometryVenusPointer->setMeshObject(planet_object);
  geometryVenusPointer->setName("geometryVenus");
  root.addChildren(venusPointer);
  venus.addChildren(geometryVenusPointer);
  venus.setParent(rootPointer);
  geometryVenus.setParent(venusPointer);

  // earth

  Node earth = Node(rootPointer, "earth");
  std::shared_ptr<Node> earthPointer = std::make_shared<Node>(earth);

  GeometryNode geometryEarth = GeometryNode(earthPointer, "geometryEarth");
  std::shared_ptr<GeometryNode> geometryEarthPointer = std::make_shared<GeometryNode>(geometryEarth);

  geometryEarthPointer->setMeshObject(planet_object); 
  geometryEarthPointer->setName("geometryEarth");
  root.addChildren(earthPointer);
  earth.addChildren(geometryEarthPointer);
  earth.setParent(rootPointer);
  geometryEarth.setParent(earthPointer);

  // moon

  Node moon = Node(earthPointer, "moon");
  std::shared_ptr<Node> moonPointer = std::make_shared<Node>(moon);

  GeometryNode geometryMoon = GeometryNode(moonPointer, "geometryMoon");
  std::shared_ptr<GeometryNode> geometryMoonPointer = std::make_shared<GeometryNode>(geometryMoon);

  geometryMoonPointer->setMeshObject(planet_object);
  geometryMoonPointer->setName("geometryMoon");
  earth.addChildren(moonPointer);
  moon.addChildren(geometryMoonPointer);
  moon.setParent(earthPointer);
  geometryMoon.setParent(moonPointer);

  // mars

  Node mars = Node(rootPointer, "mars");
  std::shared_ptr<Node> marsPointer = std::make_shared<Node>(mars);

  GeometryNode geometryMars = GeometryNode(marsPointer, "geometryMars");
  std::shared_ptr<GeometryNode> geometryMarsPointer = std::make_shared<GeometryNode>(geometryMars);

  geometryMarsPointer->setMeshObject(planet_object);
  geometryMarsPointer->setName("geometryMars");
  root.addChildren(marsPointer);
  mars.addChildren(geometryMarsPointer);
  mars.setParent(rootPointer);
  geometryMars.setParent(marsPointer);

  // jupiter

  Node jupiter = Node(rootPointer, "jupiter");
  std::shared_ptr<Node> jupiterPointer = std::make_shared<Node>(jupiter);

  GeometryNode geometryJupiter = GeometryNode(jupiterPointer, "geometryJupiter");
  std::shared_ptr<GeometryNode> geometryJupiterPointer = std::make_shared<GeometryNode>(geometryJupiter);

  geometryJupiterPointer->setMeshObject(planet_object);
  geometryJupiterPointer->setName("geometryJupiter");
  root.addChildren(jupiterPointer);
  jupiter.addChildren(geometryJupiterPointer);
  jupiter.setParent(rootPointer);
  geometryJupiter.setParent(jupiterPointer);

  // saturn

  Node saturn = Node(rootPointer, "saturn");
  std::shared_ptr<Node> saturnPointer = std::make_shared<Node>(saturn);

  GeometryNode geometrySaturn = GeometryNode(saturnPointer, "geometrySaturn");
  std::shared_ptr<GeometryNode> geometrySaturnPointer = std::make_shared<GeometryNode>(geometrySaturn);

  geometrySaturnPointer->setMeshObject(planet_object);
  geometrySaturnPointer->setName("geometrySaturn");
  root.addChildren(saturnPointer);
  saturn.addChildren(geometrySaturnPointer);
  saturn.setParent(rootPointer);
  geometrySaturn.setParent(saturnPointer);
  
  // uranus

  Node uranus = Node(rootPointer, "uranus");
  std::shared_ptr<Node> uranusPointer = std::make_shared<Node>(uranus);

  GeometryNode geometryUranus = GeometryNode(uranusPointer, "geometryUranus");
  std::shared_ptr<GeometryNode> geometryUranusPointer = std::make_shared<GeometryNode>(geometryUranus);

  geometryUranusPointer->setMeshObject(planet_object);
  geometryUranusPointer->setName("geometryUranus");
  root.addChildren(uranusPointer);
  uranus.addChildren(geometryUranusPointer);
  uranus.setParent(rootPointer);
  geometryUranus.setParent(uranusPointer);

  // neptun

  Node neptun = Node(rootPointer, "neptun");
  std::shared_ptr<Node> neptunPointer = std::make_shared<Node>(neptun);

  GeometryNode geometryNeptun = GeometryNode(neptunPointer, "geometryNeptun");
  std::shared_ptr<GeometryNode> geometryNeptunPointer = std::make_shared<GeometryNode>(geometryNeptun);

  geometryNeptunPointer->setMeshObject(planet_object);
  geometryNeptunPointer->setName("geometryNeptun");
  root.addChildren(neptunPointer);
  neptun.addChildren(geometryNeptunPointer);
  neptunPointer->setParent(rootPointer);
  std::cout << "neptunPointer: " << neptunPointer->getParent()->getName() << std::endl;
  std::cout << "child neptun: " << neptunPointer->getChild("neptun")->getName() << std::endl;
  geometryNeptunPointer->setParent(neptunPointer);
  std::cout << "geometryNeptunPointer: " << geometryNeptunPointer->getParent()->getName() << std::endl;

  // pluto

  Node pluto = Node(rootPointer, "pluto");
  std::shared_ptr<Node> plutoPointer = std::make_shared<Node>(pluto);

  GeometryNode geometryPluto = GeometryNode(plutoPointer, "geometryPluto");
  std::shared_ptr<GeometryNode> geometryPlutoPointer = std::make_shared<GeometryNode>(geometryPluto);

  geometryPlutoPointer->setMeshObject(planet_object);
  geometryPlutoPointer->setName("geometryPluto");
  root.addChildren(plutoPointer);
  pluto.addChildren(geometryPlutoPointer);
  pluto.setParent(rootPointer);
  geometryPluto.setParent(plutoPointer);

  planetGraph_.addNode(sunPointer);
  planetGraph_.addNode(geometryMercuryPointer);
  planetGraph_.addNode(geometryVenusPointer);
  planetGraph_.addNode(geometryEarthPointer);
  planetGraph_.addNode(geometryMoonPointer);
  planetGraph_.addNode(geometryMarsPointer);
  planetGraph_.addNode(geometryJupiterPointer);
  planetGraph_.addNode(geometrySaturnPointer);
  planetGraph_.addNode(geometryUranusPointer);
  planetGraph_.addNode(geometryNeptunPointer);
  planetGraph_.addNode(geometryPlutoPointer);

}


void ApplicationSolar::traverseSolarSystem() {

  makeSolarSystem();

  std::cout << "traverse1" << std::endl;
  std::cout << planetGraph_.getNodes().size() << std::endl;

  for (std::shared_ptr<GeometryNode> node : planetGraph_.getNodes()) {
    std::cout << "for" << std::endl;

    // translate values set the direction of the rotation 
    node->getWorldTransform() = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    node->getWorldTransform() = glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -1.0f});
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                      1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

    // extra matrix for normal transformation to keep them orthogonal to surface
    node->getLocalTransform() = glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform());
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                      1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

    // bind the VAO to draw
    glBindVertexArray(node->getMeshObject().vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

    // std::cout << "hallo" << std::endl;
    // std::cout << node->getName() << std::endl;
    // // std::cout << node->getParent() << std::endl;
    // std::cout << glm::to_string(node->getLocalTransform()) << std::endl;
    // std::cout << glm::to_string(node->getWorldTransform()) << std::endl;
  }

  // std::cout << "traverse2" << std::endl;

}


void ApplicationSolar::createPlanets() {

  makeSolarSystem();

// translate values set the direction of the rotation 

  auto list = planetGraph_.getNodes();
  auto sun = list.front()->getName();
  std::cout << "sun: " << sun << std::endl;
  std::shared_ptr<GeometryNode> earth;

  // for (std::shared_ptr<GeometryNode> node : planetGraph_.getNodes()) {
  //   if (node->getName().compare("geometryEarth") == 0) {
  //     earth = node;
  //   }
  // }

  auto it = list.begin();
  std::advance(it, 3);
  earth = *it;
  std::cout << "hopefully earth: " << earth->getName() << std::endl;

  for (std::shared_ptr<GeometryNode> node : planetGraph_.getNodes()) {

    if (node->getName().compare("sun") == 0) {
      
      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{2.5f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, 0.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryMercury") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);


      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()) * 2.0f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.3f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -9.5f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryVenus") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()) * 1.7f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.5f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -10.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryEarth") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()) * 1.3f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.5f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -11.5f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryMoon") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(earth->getWorldTransform(), float(glfwGetTime()) * 0.5f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.2f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -11.5f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryMars") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()) * 1.1f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.4f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -13.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryJupiter") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()) * 0.8f, glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{1.3f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -14.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometrySaturn") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime() * 0.6f), glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.9f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -15.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryNeptun") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime() * 0.4f), glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.75f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -16.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }

    else if (node->getName().compare("geometryPluto") == 0) {

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);

      node->setWorldTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime() * 0.3f), glm::fvec3{0.0f, 1.0f, 0.0f}));
      node->setWorldTransform(glm::scale(node->getWorldTransform(), glm::vec3{0.3f}));
      node->setWorldTransform(glm::translate(node->getWorldTransform(), glm::fvec3{0.0f, 0.0f, -70.0f}));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

      std::cout << "name: " << node->getName() << std::endl;
      std::cout << "local: " << glm::to_string(node->getLocalTransform()) << std::endl;
      std::cout << "world: " << glm::to_string(node->getWorldTransform()) << std::endl;

    }


  }

}





void ApplicationSolar::render() {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  // translate values set the direction of the rotation 
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  // glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  // std::cout << "render1" << std::endl;

  // traverseSolarSystem();

  createPlanets();

  // std::cout << "render2" << std::endl;
}





void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  std::cout << "hallo" << std::endl;
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}