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
uniform vec3 camera;
// boolean if outline of planets should be drawn
uniform bool Outline;

vec3 LIGHT_AMBIENT = vec3(1.0, 1.0, 1.0);
vec3 LIGHT_DIFFUSE = vec3(1.0, 1.0, 1.0);
vec3 LIGHT_SPECULAR = vec3(1.0, 1.0, 1.0);
float SHININESS = 20.0f;
vec4 result;
vec4 OUTLINE_COLOR = vec4(0.0, 1.0, 1.0, 1.0);
/* Outline thickness for unlit areas */
float UnlitOutlineThickness = 0.1;
/* Outline thickness for lit areas */
float LitOutlineThickness = 0.2;


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

  // calculating v vector (view vector)
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

  if (Outline) {
      //if (dot(normal, v) < 0.01) {
      //if (dot(v, normal) < mix(UnlitOutlineThickness, LitOutlineThickness, max(0, dot(normal, l)))) {
        // out_Color = OUTLINE_COLOR;
        if (dot(normal, v) > 0.6)
          out_Color = vec4(1.0, 1.0, 1.0, 1.0);
        else if (dot(normal, v) > 0.2)
          out_Color = vec4(0.8, 0.8, 0.8, 1.0);
        else if (dot(normal, v) > 0.1)
          out_Color = vec4(0.6, 0.6, 0.6, 1.0);
        else if (dot(normal, v) > -0.4)
          out_Color = vec4(0.45, 0.45, 0.45, 1.0);
        else if (dot(normal, v) > -0.7)
          out_Color = vec4(0.33, 0.33, 0.33, 1.0);
        else
          out_Color = vec4(0.2, 0.2, 0.2, 1.0);
      //} else {
      //  out_Color = result;
      //}
  } else {   
    out_Color = result;
  }
}
