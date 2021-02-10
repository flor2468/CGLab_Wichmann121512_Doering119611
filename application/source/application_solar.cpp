#include "application_solar.hpp"

//constant value for number of stars drawn
const int STARCOUNT = 1000;
//vector of stars
std::vector<float> stars {};

// location of uniform variable "Cel"
static GLuint g_locationCel;
// boolen if cel is active
GLboolean g_cel = GL_FALSE;

/** location of uniform-variable "Texture" */
static GLuint g_locationTexture;
/** boolean if texture is active */
GLboolean g_texture = GL_TRUE;

/** location of uniform-variable "NormalTexture" */
static GLuint g_locationNormalTexture;
/** boolean if normal texture is active */
GLboolean g_normal_texture = GL_FALSE;

/** boolean if luminance preserving grayscale is active */
GLboolean g_grayscale = GL_FALSE;
/** boolean if horizontal mirroring is active */
GLboolean g_horizontalMirroring = GL_FALSE;
/** boolean if vertical mirroring is active */
GLboolean g_verticalMirroring = GL_FALSE;
/** boolean if blur is active */
GLboolean g_blur = GL_FALSE;
/** location of uniform variable "Processing" */
static GLuint g_processing_location;
/** location of uniform variable "Gray" */
static GLuint g_locationGray;
/** location of uniform variable "HorizontalMirroring" */
static GLuint g_locationHorizontalMirroring;
/** location of uniform variable "VerticalMirroring" */
static GLuint g_locationVerticalMirroring;
/** location of uniform variable "Blur" */
static GLuint g_locationBlur;


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}, star_object{}
 // sets the distance of the planet, the bigger the (z) value, the greater the distance
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 30.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}, sceneGraph_{}
{
  initializeGeometry();
  initializeShaderPrograms();
  makeSolarSystem();
  initializePlanets();
  initializeTexture();
  initializeStars();
  // initial_resolution is size of window -> defined in framework/application.cpp
  initializeFramebuffer(initial_resolution.x, initial_resolution.y);
  initializeFullScreenQuad();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


/**RENDER FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------**/


void ApplicationSolar::render() {
  
  glBindFramebuffer(GL_FRAMEBUFFER, framebuff_object.handle);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  drawPlanets();
  drawLight();
  drawStars();
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  drawFullScreenQuad();
}


void ApplicationSolar::drawFullScreenQuad() {

  glUseProgram(m_shaders.at("fullScreenQuad").handle);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuff_object.handle_texture);

  // add sampler
  //int g_locationScreenTexture = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "TextureScreenQuad");
  //glUniform1i(g_locationScreenTexture, 0);

   // upload the processing unit data to the shader
      g_processing_location = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "Processing");
      //glUseProgram(m_shaders.at("fullScreenQuad").handle);
      glUniform1i(g_processing_location, 0);

  //render quad
  glBindVertexArray(full_screen_quad_object.vertex_AO);
  glDrawArrays(full_screen_quad_object.draw_mode, 0, full_screen_quad_object.num_elements);
}


void ApplicationSolar::drawPlanets() {
  // save every nodes from the list "nodes" of the scene graph in the variable "list"
  auto list = sceneGraph_.getNodes();
  
  // create the pointer "earth" for getting access to it for setting the WorldTransform-matrice of the moon later
  std::shared_ptr<Node> earth;

  auto it = list.begin();
  std::advance(it, 3);
  earth = *it;

  // over every node in the scenegraph should be iterated
  for (std::shared_ptr<Node> node : sceneGraph_.getNodes()) {
    
    // bind shader to upload uniforms
      glUseProgram(m_shaders.at("planet").handle);
      
      /** SET SPEED, SIZE AND POSITION **/

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

      /** SET COLOR **/

      auto temp = glGetUniformLocation(m_shaders.at("planet").handle, "planet_Color");
      glUniform3f(temp, node->getColor()[0], node->getColor()[1], node->getColor()[2]);

      /** SET TEXTURES **/

      // for texturing *************************************************************************************************

      // enables the function that with pressing "T" the textures are visible or the colors
      g_locationTexture = glGetUniformLocation(m_shaders.at("planet").handle, "Texture");
      glUniform1i(g_locationTexture, g_texture);

      // getting the index of the textures
      int indexOfTexture = node->getIndex();

      // getting the textureObject of each planet
      texture_object textureObject = sceneGraph_.getSingleTextureObject(indexOfTexture);

      // binding the textures for accessing
      glActiveTexture(GL_TEXTURE0 + indexOfTexture);

      glBindTexture(textureObject.target, textureObject.handle);

      // upload the texture unit data to the shader
      auto texture_location = glGetUniformLocation(m_shaders.at("planet").handle, "TexturePlanet");

      glUseProgram(m_shaders.at("planet").handle);

      glUniform1i(texture_location, textureObject.handle);

      // for normal mapping *************************************************************************************************

      // enables the function that with pressing "M" the normal textures are visible or the colors
      g_locationNormalTexture = glGetUniformLocation(m_shaders.at("planet").handle, "NormalTexture");
      glUniform1i(g_locationNormalTexture, g_normal_texture);

      // getting the index of the normal-textures
      int indexOfNormal = node->getNormalIndex();

      // getting the normalObject of each planet
      texture_object normalObject = sceneGraph_.getSingleNormalTextureObject(indexOfNormal);

      // binding the textures for accessing
      glActiveTexture(GL_TEXTURE0 + indexOfNormal);

      glBindTexture(normalObject.target, normalObject.handle);

      // upload the texture unit data to the shader
      auto normal_texture_location = glGetUniformLocation(m_shaders.at("planet").handle, "NormalTexturePlanet");

      glUseProgram(m_shaders.at("planet").handle);

      glUniform1i(normal_texture_location, normalObject.handle);

      // for cel-shading *************************************************************************************************

      // enables the function that with pressing "O" the outlines are visible or the colors
      g_locationCel = glGetUniformLocation(m_shaders.at("planet").handle, "Cel");
      // information if cel should be drawn to shader 
      glUniform1i(g_locationCel, g_cel);

      /** BINDING AND DRAWING **/

      // bind the VAO to draw
      glBindVertexArray(node->getMeshObject().vertex_AO);

      // drawing the planets
      glDrawElements(node->getMeshObject().draw_mode, node->getMeshObject().num_elements, model::INDEX.type, NULL);
  }
}


void ApplicationSolar::drawLight()  {

  for (auto lightNode : sceneGraph_.getLightNodes()) {

    auto intensity_temp = glGetUniformLocation(m_shaders.at("planet").handle, "light_Intensity");
    glUniform1f(intensity_temp, lightNode->getLightIntensity());

    auto color_temp = glGetUniformLocation(m_shaders.at("planet").handle, "light_Color");    
    glUniform3f(color_temp, lightNode->getColor()[0], lightNode->getColor()[1], lightNode->getColor()[2]);

    auto position_temp = glGetUniformLocation(m_shaders.at("planet").handle, "light_Position");    
    glUniform3f(position_temp, lightNode->getPosition()[0], lightNode->getPosition()[1], lightNode->getPosition()[2]);
  
  }
}


void ApplicationSolar::drawStars()  {

  // bin shader to upload the uniforms of the stars
  glUseProgram(m_shaders.at("star").handle);

  glm::mat4 identity = glm::mat4();

  // drawing each star
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(identity));

  glBindVertexArray(star_object.vertex_AO);

  glDrawArrays(star_object.draw_mode, GLint(0), star_object.num_elements);

}


void ApplicationSolar::drawSkyBox() {



}


/**UPLOAD FUNCTIONS -----------------------------------------------------------------------------------------------------------------------------------------**/


void ApplicationSolar::uploadView(std::string shader) {
  
  std::cout << g_grayscale << std::endl;
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

  glUseProgram(m_shaders.at("fullScreenQuad").handle);

  // enables the function that with pressing "7" the luminance preserving grayscale is visible
  g_locationGray = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "Gray");
  glUniform1i(g_locationGray, g_grayscale);

  // enables the function that with pressing "8" the horizontal mirroring is visible
  g_locationHorizontalMirroring = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "HorizontalMirroring");
  glUniform1i(g_locationHorizontalMirroring, g_horizontalMirroring);

  // enables the function that with pressing "9" the vertical mirroring is visible
  g_locationVerticalMirroring = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "VerticalMirroring");
  glUniform1i(g_locationVerticalMirroring, g_verticalMirroring);

  // enables the function that with pressing "0" the blur is visible
  g_locationBlur = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "Blur");
  glUniform1i(g_locationBlur, g_blur);
}


void ApplicationSolar::initializeFramebuffer(int width, int height) {

  // generating the frambuffer object
  glGenFramebuffers(1, &framebuff_object.handle);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuff_object.handle);

  // creating the color attachment with a texture object
  texture_object col_attach;

  // initialise texture
  glActiveTexture(GL_TEXTURE0);

  glGenTextures(1, &col_attach.handle);

  col_attach.target = GL_TEXTURE_2D;

  glBindTexture(col_attach.target, col_attach.handle);

  // define the texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // define the texture data and its format
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, col_attach.handle, 0);

  framebuff_object.color_attachment = col_attach;
  framebuff_object.handle_texture = col_attach.handle;

  glGenRenderbuffers(1, &framebuff_object.handle_renderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, framebuff_object.handle_renderbuffer);

  // must be a color-renderable, depth-renderable, or stencil-renderable format -> https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glRenderbufferStorage.xhtml
  // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTexImage2D.xml
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuff_object.handle_renderbuffer);

  // Define which Buffers to Write
  GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);

  // checking if framebuffer could be generated
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Could not write framebuffer." << std::endl;
  }

  else {
    std::cout << "\n Writing framebuffer successfull." << std::endl;
  }
}


void ApplicationSolar::initializeFullScreenQuad() {

  // creating screen quad like in the slides
  std::vector<GLfloat> screenQuad = {

    // first triangle

    // v4
    -1.0f, 1.0f, 0.0f, 1.0f,
    // v1
    -1.0f, -1.0f, 0.0f, 0.0f,
    // v2
    1.0f, -1.0f, 1.0f, 0.0f,

    // second triangle

    // v4
    -1.0f, 1.0f, 0.0f, 1.0f,
    // v2
    1.0f, -1.0f, 1.0f, 0.0f,
    // v3
    1.0f, 1.0f, 1.0f, 1.0f
  };

  // initializing the screen quad object like the star object

  // Initialise Vertex Array Object
  glGenVertexArrays(1, &full_screen_quad_object.vertex_AO);
  glBindVertexArray(full_screen_quad_object.vertex_AO);

  // Initialise Vertex Buffer Object and load data
  glGenBuffers(1, &full_screen_quad_object.vertex_BO);
  glBindBuffer(GL_ARRAY_BUFFER, full_screen_quad_object.vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*screenQuad.size(), screenQuad.data(), GL_STATIC_DRAW);
  
  // Specify (activate, connect and set format) the Attributes..
  //..for position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 4), 0);
  
  // ...for color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float)* 4), (void*) (sizeof(float)*2));

  // set the draw_mode of the screenQuad_object to GL_POINTS, because each screenQuad is represented by triangle
  full_screen_quad_object.draw_mode = GL_TRIANGLE_STRIP; 
  full_screen_quad_object.num_elements = GLsizei(screenQuad.size() / 4);


}


/**INITIALIZATION FUNCTIONS ----------------------------------------------------------------------------------------------------------------------------------**/


void ApplicationSolar::makeSolarSystem() {

  // create the nodes for the scene graph 
  // every node represents a planet (or a moon or the sun)

  // root

  Node root = Node(nullptr, "root");
  std::shared_ptr<Node> rootPointer = std::make_shared<Node>(root);

  // SceneGraph

  sceneGraph_ = SceneGraph("scene1", rootPointer);

  // camera

  CameraNode camera = CameraNode();
  std::shared_ptr<CameraNode> cameraPointer = std::make_shared<CameraNode>(camera);
  sceneGraph_.setCamera(cameraPointer);

  // light

  PointLightNode light = PointLightNode();  
  std::shared_ptr<PointLightNode> lightPointer = std::make_shared<PointLightNode>(light);

  lightPointer->setColor({1.0f, 1.0f, 0.7f});

  root.addChildren(lightPointer);

  sceneGraph_.addLightNode(lightPointer);

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
  sceneGraph_.addNode(sunPointer);
  sceneGraph_.addNode(geometryMercuryPointer);
  sceneGraph_.addNode(geometryVenusPointer);
  sceneGraph_.addNode(geometryEarthPointer);
  sceneGraph_.addNode(geometryMoonPointer);
  sceneGraph_.addNode(geometryMarsPointer);
  sceneGraph_.addNode(geometryJupiterPointer);
  sceneGraph_.addNode(geometrySaturnPointer);
  sceneGraph_.addNode(geometryUranusPointer);
  sceneGraph_.addNode(geometryNeptunPointer);
  sceneGraph_.addNode(geometryPlutoPointer);
  sceneGraph_.setSize(sceneGraph_.getNodes().size());

}


void ApplicationSolar::initializePlanets() {

  int counter = 0;

  // auto temp = glGetUniformLocation(m_shaders.at("planet").handle, "planet_Color");

  // over every node in the scenegraph should be iterated
  for (std::shared_ptr<Node> node : sceneGraph_.getNodes()) {

    
    if (node->getName().compare("sun") == 0) {

      node->setSize({2.0f, 2.0f, 2.0f});
      node->setColor({1.0f, 1.0f, 0.0f});     
      node->setIndex(1);

    } else if (node->getName().compare("geometryMercury") == 0) {

      node->setSpeed(1.7f);
      node->setSize({0.3f, 0.3f, 0.3f});
      node->setPosition({0.0f, 0.0f, -9.5f});
      node->setColor({0.5f, 0.4f, 0.4f});
      node->setIndex(2);
    
     } else if (node->getName().compare("geometryVenus") == 0) {

      node->setSpeed(1.7f);
      node->setSize({0.5f, 0.5f, 0.5f});
      node->setPosition({0.0f, 0.0f, -10.0f});
      node->setColor({0.9f, 0.5f, 0.0f});
      node->setIndex(3);

    } else if (node->getName().compare("geometryEarth") == 0) {

      node->setSpeed(1.3f);
      node->setSize({0.5f, 0.5f, 0.5f});
      node->setPosition({0.0f, 0.0f, -11.5f});
      node->setColor({0.3f, 0.7f, 0.0f});
      node->setIndex(4);

    } else if (node->getName().compare("geometryMars") == 0) {

      node->setSpeed(1.1f);
      node->setSize({0.4f, 0.4f, 0.4f});
      node->setPosition({0.0f, 0.0f, -13.0f});
      node->setColor({0.8f, 0.0f, 0.1f});
      node->setIndex(6);

    } else if (node->getName().compare("geometryJupiter") == 0) {

      node->setSpeed(0.8f);
      node->setSize({1.3f, 1.3f, 1.3f});
      node->setPosition({0.0f, 0.0f, -14.0f});
      node->setColor({0.7f, 0.7f, 0.6f});
      node->setIndex(7);

    } else if (node->getName().compare("geometrySaturn") == 0) {

      node->setSpeed(0.6f);
      node->setSize({0.9f, 0.9f, 0.9f});
      node->setPosition({0.0f, 0.0f, -15.0f});
      node->setColor({0.9f, 0.8f, 0.4f});
      node->setIndex(8);

    } else if (node->getName().compare("geometryUranus") == 0) {

      node->setSpeed(0.5f);
      node->setSize({0.8f, 0.8f, 0.8f});
      node->setPosition({0.0f, 0.0f, -15.5f});
      node->setColor({0.7f, 0.0f, 0.5f});
      node->setIndex(9);

    } else if (node->getName().compare("geometryNeptun") == 0) {

      node->setSpeed(0.4f);
      node->setSize({0.75f, 0.75f, 0.75f});
      node->setPosition({0.0f, 0.0f, -16.0f});
      node->setColor({0.0f, 0.4f, 1.0f});
      node->setIndex(10);
      
    } else if (node->getName().compare("geometryPluto") == 0) {

      node->setSpeed(0.3f);
      node->setSize({0.3f, 0.3f, 0.3f});
      node->setPosition({0.0f, 0.0f, -70.0f});
      node->setColor({0.4f, 0.3f, 0.0f});
      node->setIndex(11);

    } else if (node->getName().compare("geometryMoon") == 0) {

      node->setSpeed(0.5f);
      node->setSize({0.2f, 0.2f, 0.2f});
      node->setPosition({0.0f, 0.0f, -11.5f});
      node->setColor({0.5f, 0.5f, 0.5f});
      node->setIndex(5);
    
    }
    node->setNormalIndex(sceneGraph_.getSize() + node->getIndex());
  }
}


void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("planet").u_locs["TexturePlanet"] = -1;
  m_shaders.at("planet").u_locs["NormalTexturePlanet"] = -1;

  // creating shader program for "star"
  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  m_shaders.at("star").u_locs["ModelMatrix"] = -1;
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

  // shader program for the full screen quad
  m_shaders.emplace("fullScreenQuad", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/screenQuad.vert"},
                                                   {GL_FRAGMENT_SHADER, m_resource_path + "shaders/screenQuad.frag"}}});

  m_shaders.at("fullScreenQuad").u_locs["Processing"] = -1;
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


void ApplicationSolar::initializeGeometry() {

  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);

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

  glEnableVertexAttribArray(2);
  // third attribute is 2 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

  glEnableVertexAttribArray(3);
  // third attribute is 2 floats with no offset & stride
  glVertexAttribPointer(3, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

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


void ApplicationSolar::initializeSkyBox() {




}


void ApplicationSolar::initializeTexture() {

  // getting pixel data for the textures and the normal-textures
  pixel_data textureOfPlanet;
  pixel_data normalOfPlanet;

  // for loop for the textures **************************************************************************************************

  for (auto node : sceneGraph_.getNodes()) {

    // getting the index of the texture for each node
    int indexOfTexture = node->getIndex();

    // loading the texture images -> if not possible through an exception
    try {

      textureOfPlanet = texture_loader::file(m_resource_path + "textures/" + node->getName() + ".png");
    }

    catch (std::exception exception) {

      std::cout << "Could not load the texture of " + node->getName() + "\n" + exception.what() << std::endl;

    }

    // like in the texture_loader.cpp we need width and height & like in pixel_data.hpp we need channels and channel_type

    // width & height are the dimensions of the image (valid for 2D)
    GLsizei width = (GLsizei)textureOfPlanet.width;
    GLsizei height = (GLsizei)textureOfPlanet.height;

    // (internal) format of the texture/ texel data
    GLenum numOfChannel = textureOfPlanet.channels;

    // data type of the texel data
    GLenum typeOfChannel = textureOfPlanet.channel_type;

    // initialise texture
    glActiveTexture(GL_TEXTURE0 + indexOfTexture);

    texture_object textureObject;

    glGenTextures(1, &textureObject.handle);

    textureObject.target = GL_TEXTURE_2D;

    // adding each textureObject to the list, where they can be found by its index
    sceneGraph_.addTextureObjects({indexOfTexture, textureObject});

    glBindTexture(textureObject.target, textureObject.handle);

    // define the texture sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // define the texture data and its format
    glTexImage2D(GL_TEXTURE_2D, 0, numOfChannel, width, height, 0, numOfChannel, typeOfChannel, textureOfPlanet.ptr());

  }

  // for loop for the normal-textures ******************************************************************************************

  for (auto node : sceneGraph_.getNodes()) {

    // getting the index of the normal-texture for each node
    int indexOfNormal = node->getNormalIndex();

    // loading the normal-texture images -> if not possible through an exception
    try {

      normalOfPlanet = texture_loader::file(m_resource_path + "textures/" + node->getName() + "_normalmap.png");
    }

    catch (std::exception exception) {

      std::cout << "Could not load the normalmap of " + node->getName() + "\n" + exception.what() << std::endl;

    }

    // like in the texture_loader.cpp we need width and height & like in pixel_data.hpp we need channels and channel_type

    // width & height are the dimensions of the image (valid for 2D)
    GLsizei n_width = (GLsizei)normalOfPlanet.width;
    GLsizei n_height = (GLsizei)normalOfPlanet.height;

    // (internal) format of the texture/ texel data
    GLenum n_numOfChannel = normalOfPlanet.channels;

    // data type of the texel data
    GLenum n_typeOfChannel = normalOfPlanet.channel_type;

    // initialise texture
    glActiveTexture(GL_TEXTURE0 + indexOfNormal);

    texture_object normalObject;

    glGenTextures(1, &normalObject.handle);

    normalObject.target = GL_TEXTURE_2D;

    // adding each textureObject to the list, where they can be found by its index
    sceneGraph_.addNormalTextureObjects({indexOfNormal, normalObject});

    glBindTexture(normalObject.target, normalObject.handle);

    // define the texture sampling parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // define the texture data and its format
    glTexImage2D(GL_TEXTURE_2D, 0, n_numOfChannel, n_width, n_height, 0, n_numOfChannel, n_typeOfChannel, normalOfPlanet.ptr());
  }
}


/**BOOLEAN FUNCTIONS FOR DE(ACTIVATING) EVENTS ---------------------------------------------------------------------------------------------------------------**/


// (de)activates the texture mode
void ApplicationSolar::toggleTexture () {
    g_texture = !g_texture;
}


// (de)activates the normal texture mode
void ApplicationSolar::toggleNormalTexture () {
    g_normal_texture = !g_normal_texture;
}

// (de)activates the cel shading mode
void ApplicationSolar::toggleCel() {
  /* change state */
  g_cel = !g_cel;
}

// (de)activates the gray scale mode
void ApplicationSolar::toggleGrayscale() {
  g_grayscale = !g_grayscale;
}

// (de)activates the horizontal mirroring mode
void ApplicationSolar::toggleHorizontalMirroring() {
  g_horizontalMirroring = !g_horizontalMirroring;
}

// (de)activates the vertical mirroring mode
void ApplicationSolar::toggleVerticalMirroring() {
  g_verticalMirroring = g_verticalMirroring;
}

// (de)activates the blur mode
void ApplicationSolar::toggleBlur() {
  g_blur = !g_blur;
}


/**CALLBACK FUNCTIONS -----------------------------------------------------------------------------------------------------------------------------------------**/


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

  if (key == GLFW_KEY_O && action == GLFW_PRESS) {
    toggleCel();
  }

  if (key == GLFW_KEY_T && action == GLFW_PRESS) {
    toggleTexture();
  }

  if (key == GLFW_KEY_M && action == GLFW_PRESS) {
    toggleNormalTexture();
  }

  if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
    toggleGrayscale();
  }

  if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
    toggleHorizontalMirroring();
  }

  if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
    toggleVerticalMirroring();
  }

  if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
    toggleBlur();
  }


  // actualize the shaders and also view matrix of "planet" and "star" and "fullScreenQuad"

  glUseProgram(m_shaders.at("planet").handle);
  uploadView("planet");

  glUseProgram(m_shaders.at("star").handle);
  uploadView("star");

  glUseProgram(m_shaders.at("fullScreenQuad").handle);
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

/** ------------------------------------------------------------------------------------------------------------------------------------------------------- **/