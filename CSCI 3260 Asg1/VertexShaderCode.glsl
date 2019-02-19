#version 440

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 transformMatrix;
uniform mat4 projectionMatrix;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 new_position = transformMatrix * v;
	vec4 projected_position = projectionMatrix * new_position;
	gl_Position = projected_position;	
	theColor = vertexColor;
}