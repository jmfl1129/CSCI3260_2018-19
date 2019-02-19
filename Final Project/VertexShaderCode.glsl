#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;


uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelScalingMatrix;
uniform mat4 modelRotationMatrix;
uniform mat4 modelRotationMatrix1;

out vec2 UV;
out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 new_position = projectionMatrix * modelTransformMatrix * modelRotationMatrix1 * modelRotationMatrix * modelScalingMatrix * v;
	gl_Position = new_position;	

	vec4 normal_temp = projectionMatrix * modelTransformMatrix * modelRotationMatrix1 *  modelRotationMatrix * modelScalingMatrix * vec4(normal, 0);
	normalWorld = normal_temp.xyz;
	
	vertexPositionWorld = new_position.xyz;

	UV = vertexUV;
}