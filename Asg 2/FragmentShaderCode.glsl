#version 440

out vec3 color;
in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 ambientLight;
uniform vec3 lightPositionWorld;
uniform float diffuseBrightness;
uniform float specularBrightness;
uniform float spotBrightness;
uniform float cutoffangle;
uniform vec3 spotPositionWorld;
uniform vec3 eyePositionWorld;

uniform sampler2D myTextureSampler;
void main()
{
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld)) * diffuseBrightness;
	vec3 diffuseLight = vec3 (brightness, brightness, brightness);

	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	vec3 specularLight = vec3(s * specularBrightness, s * specularBrightness, s * specularBrightness);

	vec3 spotVectorWorld = normalize(spotPositionWorld - vertexPositionWorld);
	float spot = dot(spotVectorWorld, normalize(normalWorld));
	vec3 spotLight = vec3(spot * spotBrightness, spot * spotBrightness, spot * spotBrightness);
	if (spot > cutoffangle)
	{
	color = texture( myTextureSampler, UV ).rgb * (ambientLight + clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1) + clamp(spotLight, 0, 1));
	}
	else
	color = texture( myTextureSampler, UV ).rgb * (ambientLight + clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1));
}
