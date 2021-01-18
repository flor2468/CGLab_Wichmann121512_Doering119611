#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "SceneGraph.hpp"
#include "GeometryNode.hpp"
#include "CameraNode.hpp"
#include "PointLightNode.hpp"
#include "SceneGraph.hpp"
#include "window_handler.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "star.hpp"
#include "external/stb_image-2.0.6/stb_image.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <random>



#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();
  //activates/ deactivates the drawing of the planet outlines
  void toggleCel();
  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render();

  void makeSolarSystem();
  
  void initializePlanets();
  void drawPlanets();
  void initializeStars();
  void drawStars();
  void initializeLight();

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection(std::string shader);
  // upload view matrix
  void uploadView(std::string shader);

  // cpu representation of model
  model_object planet_object;

  model_object star_object;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  SceneGraph planetGraph_;
};

#endif