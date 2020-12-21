#version 150

in vec3 pass_Normal;
in vec3 pass_Vertex_Pos;
in vec3 pass_Camera_Pos;
in mat4 pass_ViewMatrix;

out vec4 out_Color;

uniform vec3 planet_Color;
uniform vec3 light_Color;
uniform vec3 light_Position;
uniform float light_Intensity;


vec3 LIGHT_AMBIENT = vec3(1.0, 1.0, 1.0);
vec3 LIGHT_DIFFUSE = vec3(1.0, 1.0, 1.0);
vec3 LIGHT_SPECULAR = vec3(1.0, 1.0, 1.0);
float SHININESS = 20.0f;

uniform vec3 camera;

vec4 result;


void main() {
  out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4(planet_Color, 1.0);

  // calculating the normal
  vec3 normal = normalize(pass_Normal);

  // calculating l vector
  // pos of light - pos of vertex
  vec3 actualized_Light_Position = (pass_ViewMatrix * vec4(light_Position, 1.0)).xyz;
  vec3 l = actualized_Light_Position - pass_Vertex_Pos;
  l = normalize(l);

  // calculating v vector
  // pos of camera - pos of vertex
  vec3 v = (pass_Camera_Pos - pass_Vertex_Pos);
  v = normalize(v);

  // calculate h = l + v
  vec3 h = normalize(l + v);

  // diffuse light
  vec3 diffuse_part = max(dot(normal, l), 0) * LIGHT_DIFFUSE;

  // specular light
  vec3 specular_part = pow(max(dot(h, normal), 0), SHININESS) * LIGHT_SPECULAR;

  result = vec4((LIGHT_AMBIENT + diffuse_part) * planet_Color * light_Intensity + specular_part * light_Color, 1.0);

  out_Color = result;
}
