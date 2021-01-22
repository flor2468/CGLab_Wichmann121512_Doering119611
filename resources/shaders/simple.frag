#version 150

in vec3 pass_Normal;
in vec3 pass_Vertex_Pos;
in vec3 pass_Camera_Pos;
in mat4 pass_ViewMatrix;
in vec2 pass_TexCoord;

out vec4 out_Color;

uniform vec3 planet_Color;
uniform vec3 light_Color;
uniform vec3 light_Position;
uniform float light_Intensity;
uniform vec3 camera;
// boolean if cel shading of planets should be active
uniform bool Cel;
// boolean if texture of planets should be active
uniform bool Texture;

vec3 LIGHT_AMBIENT = vec3(0.7, 0.7, 0.7); // 0.1 -> sun in exception (should be very bright)
vec3 LIGHT_DIFFUSE = vec3(1.0, 1.0, 1.0); // 1.0 
vec3 LIGHT_SPECULAR = vec3(1.0, 1.0, 1.0); // 1.0
float SHININESS = 20.0f;
vec4 colorResult;
vec4 textureResult;

// textures
uniform sampler2D TexturePlanets;
vec4 texel = texture(TexturePlanets, pass_TexCoord);



void main() {
  //out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  //out_Color = vec4(planet_Color, 1.0);

  // calculating the normal
  vec3 normal = normalize(pass_Normal);

  // calculating l vector
  // pos of light - pos of vertex
  vec3 actualized_Light_Position = (pass_ViewMatrix * vec4(light_Position, 1.0)).xyz;
  vec3 l = actualized_Light_Position - pass_Vertex_Pos;
  l = normalize(l);

  // calculating v vector (view vector)
  // pos of camera - pos of vertex
  vec3 actualized_Camera_Position = (pass_ViewMatrix * vec4(pass_Camera_Pos, 1.0f)).xyz;
  vec3 v = (actualized_Camera_Position - pass_Vertex_Pos);
  v = normalize(v);

  // calculate h = l + v
  vec3 h = normalize(l + v);

  // diffuse light
  vec3 diffuse_part = max(dot(normal, l), 0) * LIGHT_DIFFUSE;

  // specular light
  vec3 specular_part = pow(max(dot(h, normal), 0), SHININESS) * LIGHT_SPECULAR;

  colorResult = vec4((LIGHT_AMBIENT + diffuse_part) * planet_Color * light_Intensity + specular_part * light_Color, 1.0);
  textureResult = vec4((LIGHT_AMBIENT + diffuse_part) * texel.rgb + specular_part * light_Color, 1.0);
  
  // if case for the cel-shading
  if (Cel) {
        // different ranges equals different colors
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

        // blue thick line over the planet (like a ring)
        // if (abs(dot(normal, v)) < 0.2) {
        //   out_Color = vec4(0.0, 0.0, 1.0, 1.0);
        // }
   // else for texture
  } else if (Texture) {   
    out_Color = textureResult;
    // else for "normal" light/ color
  } else {   
    out_Color = colorResult;
  }
}
