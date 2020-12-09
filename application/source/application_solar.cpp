#include "application_solar.hpp"

//constant value for number of stars drawn
const int STARCOUNT = 1000;
//vector of stars
std::vector<float> stars {};

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}, star_object{}
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
void ApplicationSolar::initializePlanets() {

  // over every node in the scenegraph should be iterated
  for (std::shared_ptr<Node> node : planetGraph_.getNodes()) {

    if (node->getName().compare("sun") == 0) {

      node->setSize({2.0f, 2.0f, 2.0f});

    } else if (node->getName().compare("geometryMercury") == 0) {

      node->setSpeed(1.7f);
      node->setSize({0.3f, 0.3f, 0.3f});
      node->setPosition({0.0f, 0.0f, -9.5f});
    
     } else if (node->getName().compare("geometryVenus") == 0) {

      node->setSpeed(1.7f);
      node->setSize({0.5f, 0.5f, 0.5f});
      node->setPosition({0.0f, 0.0f, -10.0f});

    } else if (node->getName().compare("geometryEarth") == 0) {

      node->setSpeed(1.3f);
      node->setSize({0.5f, 0.5f, 0.5f});
      node->setPosition({0.0f, 0.0f, -11.5f});

    } else if (node->getName().compare("geometryMars") == 0) {

      node->setSpeed(1.1f);
      node->setSize({0.4f, 0.4f, 0.4f});
      node->setPosition({0.0f, 0.0f, -13.0f});

    } else if (node->getName().compare("geometryJupiter") == 0) {

      node->setSpeed(0.8f);
      node->setSize({1.3f, 1.3f, 1.3f});
      node->setPosition({0.0f, 0.0f, -14.0f});

    } else if (node->getName().compare("geometrySaturn") == 0) {

      node->setSpeed(0.6f);
      node->setSize({0.9f, 0.9f, 0.9f});
      node->setPosition({0.0f, 0.0f, -15.0f});

    } else if (node->getName().compare("geometryNeptun") == 0) {

      node->setSpeed(0.4f);
      node->setSize({0.75f, 0.75f, 0.75f});
      node->setPosition({0.0f, 0.0f, -16.0f})
      ;
    } else if (node->getName().compare("geometryPluto") == 0) {

      node->setSpeed(0.3f);
      node->setSize({0.3f, 0.3f, 0.3f});
      node->setPosition({0.0f, 0.0f, -70.0f});

    } else if (node->getName().compare("geometryMoon") == 0) {

      node->setSpeed(0.5f);
      node->setSize({0.2f, 0.2f, 0.2f});
      node->setPosition({0.0f, 0.0f, -11.5f});
    
    }
  }
}


void ApplicationSolar::drawPlanets() {

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
    
    // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);
      
      // every value for speed, size, position and rotMat is set in initializePlanets()

      if (node->getName().compare("geometryMoon") == 0) {
        
        // special case for the moon: needs the worldTransform-matrix from the earth to rotate around the earth
        node->setWorldTransform(glm::rotate(earth->getWorldTransform(), float(glfwGetTime() * node->getSpeed()), glm::fvec3{0.0f, 1.0f, 0.0f}));

      } else {

        // set the speed of the planet with float(glfwGetTime() * factor)
        node->setWorldTransform(glm::rotate(node->getRotMat(), float(glfwGetTime() * node->getSpeed()), glm::fvec3{0.0f, 1.0f, 0.0f}));

      }

      // set the size 
      node->setWorldTransform(glm::scale(node->getWorldTransform(), node->getSize()));

      // set the position 
      node->setWorldTransform(glm::translate(node->getWorldTransform(), node->getPosition()));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(node->getWorldTransform()));

      // extra matrix for normal transformation to keep them orthogonal to surface
      glm::fmat4 normalMatrix = glm::inverseTranspose(glm::inverse(m_view_transform) * node->getWorldTransform());

      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(normalMatrix));

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // drawing the planets
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);
  }
}


void ApplicationSolar::initializeStars() {

  // create floats for storing the values of the 
  // position and color of each star
  float x, y, z, r, g, b;
  std::vector<float> position, color;

  for (int i = 0; i < STARCOUNT; i++) {
    x = float(std::rand() % 100 - 50);
    y = float(std::rand() % 100 - 50);
    z = float(std::rand() % 100 - 50);
    r = float(std::rand() % 255 / 255.0f);
    g = float(std::rand() % 255 / 255.0f);
    b = float(std::rand() % 255 / 255.0f);
    position = {x, y, z};
    color = {r, g, b};
    
    //push back random values of star to vector 
    stars.push_back(x);
    stars.push_back(y);
    stars.push_back(z);
    stars.push_back(r);
    stars.push_back(g);
    stars.push_back(b);
  }
  drawStars();
}


void ApplicationSolar::drawStars() {

    // steps from the lecture:

    // Initialise Vertex Array Object
    glGenVertexArrays(1, &star_object.vertex_AO);
    glBindVertexArray(star_object.vertex_AO);
    // Initialise Vertex Buffer Object and load data
    glGenBuffers(1, &star_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*STARCOUNT*6, stars.data(), GL_STATIC_DRAW);
    // Specify (activate, connect and set format) the Attributes..
    //..for position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 3), 0);
    // ...for color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)* 3), (void*) (sizeof(float)*3));

    // set the draw_mode of the star_object to GL_POINTS, because each star is represented by one point
    star_object.draw_mode = GL_POINTS; 
    star_object.num_elements = GLsizei(STARCOUNT);

}


void ApplicationSolar::render() {

  // bind shader to upload the uniforms of the planets
  glUseProgram(m_shaders.at("planet").handle);

  // call of the drawPlanets() function, where every planet is created
  makeSolarSystem();
  initializePlanets();
  drawPlanets();
  
  // bin shader to upload the uniforms of the stars
  glUseProgram(m_shaders.at("star").handle);

  glm::mat4 identity = glm::mat4();

  // drawing each star
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(identity));

  glBindVertexArray(star_object.vertex_AO);

  glDrawArrays(star_object.draw_mode, GLint(0), star_object.num_elements);
}


void ApplicationSolar::uploadView(std::string shader) {
  
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  
  // upload matrix to gpu
  // using "shader" to give a string in the function for example for "planet" or "star"
  glUniformMatrix4fv(m_shaders.at(shader).u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection(std::string shader) {

  // upload matrix to gpu
  // using "shader" to give a string in the function for example for "planet" or "star"
  glUniformMatrix4fv(m_shaders.at(shader).u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

/* update uniform locations */
void ApplicationSolar::uploadUniforms() { 

  // every shader must be actualized (planet and star)
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView("planet");
  uploadProjection("planet");

  glUseProgram(m_shaders.at("star").handle);
  uploadView("star");
  uploadProjection("star");
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

  // creating shader program for "star"
  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  m_shaders.at("star").u_locs["ModelMatrix"] = -1;
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
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

  //draw stars once
  initializeStars();
  drawStars();

}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  // zooming in with the arrow key "up"
  if (key == GLFW_KEY_UP  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
  }
  // zooming out with the arrow key "down"
  else if (key == GLFW_KEY_DOWN  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
  }
  // swiping to the right with the arrow key "right"
  else if (key == GLFW_KEY_RIGHT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
  }
  // swiping to the left with the arrow key "left"
  else if (key == GLFW_KEY_LEFT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
  }
  // swiping the solar system higher with the key "U"
  else if (key == GLFW_KEY_U  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
  }
  // swiping the solar system lower with the arrow key "N"
  else if (key == GLFW_KEY_N  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.0f});
  }

  // actualize the shaders and also view matrix of "planet" and "star"

  glUseProgram(m_shaders.at("planet").handle);
  uploadView("planet");

  glUseProgram(m_shaders.at("star").handle);
  uploadView("star");

}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {

  // TODO: recomment it back to have mouse handling!

  // mouse handling
  // m_view_transform = glm::rotate(m_view_transform, glm::radians(float(pos_x / 25)), glm::vec3{0.0f, 1.0f, 0.0f});
  // m_view_transform = glm::rotate(m_view_transform, glm::radians(float(pos_y / 25)), glm::vec3{1.0f, 0.0f, 0.0f});

  // glUseProgram(m_shaders.at("planet").handle);
  // uploadView("planet");

  // glUseProgram(m_shaders.at("star").handle);
  // uploadView("star");


}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  
  // upload new projection matrix
  // actualize the shaders and also projection matrix of "planet" and "star"
  glUseProgram(m_shaders.at("planet").handle);
  uploadProjection("planet");

  glUseProgram(m_shaders.at("star").handle);
  uploadProjection("star");

}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}