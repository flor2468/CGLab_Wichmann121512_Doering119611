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
#include <vector>
#include "star.hpp"
#include <random>




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

  // create the nodes for the scene graph 
  // every node represents a planet (or a moon or the sun)

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
  geometryNeptunPointer->setParent(neptunPointer);

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

  // adding every node to the scene graph
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


void ApplicationSolar::createPlanets() {

  makeSolarSystem();

// translate values set the direction of the rotation 

  // save every nodes from the list "nodes" of the scene graph in the variable "list"
  auto list = planetGraph_.getNodes();
  
  // create the pointer "earth" for getting access to it for setting the WorldTransform-matrice of the moon later
  std::shared_ptr<Node> earth;

  auto it = list.begin();
  std::advance(it, 3);
  earth = *it;

  // over every node in the scenegraph should be iterated
  for (std::shared_ptr<Node> node : planetGraph_.getNodes()) {
    float speed = 0.0f;
    glm::vec3 size {0.0f};
    glm::fvec3 position {0.0f, 0.0f, 0.0f};
    glm::fmat4 rotMat {};

    if (node->getName().compare("sun") == 0) {
      size.x = 2.5f;
      size.y = 2.5f;
      size.z = 2.5f;
    } else if (node->getName().compare("geometryMercury") == 0) {
      speed = 2.0f;
      size.x = 0.3f;
      size.y = 0.3f;
      size.z = 0.3f;
      position.z = -9.5f;
     } else if (node->getName().compare("geometryVenus") == 0) {
      speed = 1.7f;
      size.x = 0.5f;
      size.y = 0.5f;
      size.z = 0.5f;
      position.z = -10.0f;
    } else if (node->getName().compare("geometryEarth") == 0) {
      speed = 1.3f;
      size.x = 0.5f;
      size.y = 0.5f;
      size.z = 0.5f;
      position.z = -11.5f;
    } else if (node->getName().compare("geometryMars") == 0) {
      speed = 1.1f;
      size.x = 0.4f;
      size.y = 0.4f;
      size.z = 0.4f;
      position.z = -13.0f;
    } else if (node->getName().compare("geometryJupiter") == 0) {
      speed = 0.8f;
      size.x = 1.3f;
      size.y = 1.3f;
      size.z = 1.3f;
      position.z = -14.0f;
    } else if (node->getName().compare("geometrySaturn") == 0) {
      speed = 0.6f;
      size.x = 0.9f;
      size.y = 0.9f;
      size.z = 0.9f;
      position.z = -15.0f;
    } else if (node->getName().compare("geometryNeptun") == 0) {
      speed = 0.4f;
      size.x = 0.75f;
      size.y = 0.75f;
      size.z = 0.75f;
      position.z = -16.0f;
    } else if (node->getName().compare("geometryPluto") == 0) {
      speed = 0.3f;
      size.x = 0.3f;
      size.y = 0.3f;
      size.z = 0.3f;
      position.z = -70.0f;
    } else if (node->getName().compare("geometryMoon") == 0) {
      speed = 0.5f;
      size.x = 0.2f;
      size.y = 0.2f;
      size.z = 0.2f;
      position.z = -11.5f;
      rotMat = earth->getWorldTransform();
    }

      // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);
      
      // set the speed of the planet with float(glfwGetTime() * factor)
      node->setWorldTransform(glm::rotate(rotMat, float(glfwGetTime() * speed), glm::fvec3{0.0f, 1.0f, 0.0f}));

      // set the size of 2.5f (relativly big)
      node->setWorldTransform(glm::scale(node->getWorldTransform(), size));

      // set the position in the middle of the plane
      node->setWorldTransform(glm::translate(node->getWorldTransform(), position));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      node->setLocalTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getLocalTransform()));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // drawing the "sun"
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);

    }
}


void ApplicationSolar::drawStars() {
  std::vector<Star> stars {};

  const int STARCOUNT = 10;

  float x, y, z, r, g, b;
  std::vector<float> position, color;

  for (int i = 0; i <= STARCOUNT; i++) {
    x = std::rand() % 100;
    y = std::rand() % 100;
    z = std::rand() % 100;
    r = std::rand() % 100;
    g = std::rand() % 100;
    b = std::rand() % 100;
    position = {x, y, z};
    color = {r, g, b};
    Star s = {position, color};
    
    for (int t = 0; t < 3; t++) {
      s.posColData_.push_back(position[t]);
    }

    for (int t = 0; t < 3; t++) {
      s.posColData_.push_back(color[t]);
    }

    stars.push_back(s);
  }

  std::cout << "hallo" << std::endl;

  for (int j = 0; j <= stars.size(); j++) {

    stars[j].model_ = star_object;

    // steps from the lecture:

    // Initialise Vertex Array Object
    glGenVertexArrays(1, &stars[j].model_.vertex_AO);
    glBindVertexArray(stars[j].model_.vertex_AO);
    // Initialise Vertex Buffer Object and load data
    glGenBuffers(1, &stars[j].model_.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, stars[j].model_.vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, GLsizei(sizeof(float)*STARCOUNT*6), stars[j].posColData_.data(), GL_STATIC_DRAW);
    // Specify (activate, connect and set format) the Attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)*STARCOUNT*6), 0);

    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)*STARCOUNT*6), (void*) (sizeof(float)*3));


    // Define Vertex Indices (optional)
    // glGenBuffers(1, &stars[j].model_.element_BO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stars[j].model_.element_BO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)*STARCOUNT*6, stars[j].posColData_.data(), GL_STATIC_DRAW);
;

    stars[j].model_.draw_mode = GL_POINTS; 
    stars[j].model_.num_elements = GLsizei(STARCOUNT);

    std::cout << "kuchen" << std::endl;

    // (Re)bind VAO of Geometry
    glBindVertexArray(stars[j].model_.vertex_AO);
    // Bind Shader Program(s)

    std::cout << "plaetzchen" << std::endl;

    glUseProgram(m_shaders.at("star").handle);
    // Draw Geometry
    // Using indices and parameters

    glDrawElements(stars[j].model_.draw_mode, stars[j].model_.num_elements, model::INDEX.type, NULL);
    // Using parameters but NO indices

    glDrawArrays(stars[j].model_.draw_mode, 0, stars[j].model_.num_elements);

    std::cout << "keks" << std::endl;

    // glUseProgram(m_shaders.at("star").handle);

    // glUniformMatrix4fv(m_shaders.at("star").u_locs.at("MViewMatrix"),
    //                  1, GL_FALSE, glm::value_ptr(m_view_transform));
  
    // glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
    //                  1, GL_FALSE, glm::value_ptr(m_view_projection));

    // glBindVertexArray(stars[j].model_.vertex_AO);

    // glDrawArrays(stars[j].model_.draw_mode, GLint(0), stars[j].model_.num_elements);

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

  // call of the createPlanets() function, where every planet is created
  createPlanets();

  drawStars();
}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("MViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_transform));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);

  glUseProgram(m_shaders.at("star").handle);
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

  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});

  // m_shaders.at("star").u_locs["NormalMatrix"] = -1;
  // m_shaders.at("star").u_locs["ModelMatrix"] = -1;
  // m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["MViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

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
  // zooming in with the arrow key "up"
  if (key == GLFW_KEY_UP  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  // zooming out with the arrow key "down"
  else if (key == GLFW_KEY_DOWN  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  // swiping to the right with the arrow key "right"
  else if (key == GLFW_KEY_RIGHT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
  // swiping to the left with the arrow key "left"
  else if (key == GLFW_KEY_LEFT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
    uploadView();
  }
  // swiping the solar system higher with the key "U"
  else if (key == GLFW_KEY_U  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
    uploadView();
  }
  // swiping the solar system lower with the arrow key "N"
  else if (key == GLFW_KEY_N  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.0f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling

   m_view_transform = glm::rotate(m_view_transform, glm::radians(float(pos_x / 25)), glm::vec3{0.0f, 1.0f, 0.0f});
   m_view_transform = glm::rotate(m_view_transform, glm::radians(float(pos_y / 25)), glm::vec3{1.0f, 0.0f, 0.0f});

   uploadView();

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