#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

out vec3 pass_Normal;
out vec3 pass_Vertex_Pos;
out vec3 pass_Camera_Pos;
out mat4 pass_ViewMatrix;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;

	// actualize out vectors 
	pass_Vertex_Pos = ((ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0)).xyz;
	pass_Camera_Pos = (ViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	pass_ViewMatrix = ViewMatrix;
}
