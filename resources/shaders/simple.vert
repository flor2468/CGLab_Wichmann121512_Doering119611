#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec2 in_NormTexCoord;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec3 pass_Normal;
out vec3 pass_Vertex_Pos;
out vec3 pass_Camera_Pos;
out mat4 pass_ViewMatrix;
out vec2 pass_TexCoord;
out vec2 pass_NormTexCoord;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;

	// actualize out vectors 
	pass_Vertex_Pos = ((ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0)).xyz;
	pass_Camera_Pos = (ViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	pass_ViewMatrix = ViewMatrix;
	pass_TexCoord = in_TexCoord;
	pass_NormTexCoord = in_NormTexCoord;
}
