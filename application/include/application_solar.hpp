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

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render();

  void makeSolarSystem();
  
  void createPlanets();

  void drawStars();

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