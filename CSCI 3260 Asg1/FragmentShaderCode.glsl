#version 440

out vec4 daColor;
in vec3 theColor;
uniform float Colorx;
uniform float Colory;
uniform float cancellingx;
uniform float cancellingy;

void main()
{
	daColor = vec4(theColor, 1.0);
}