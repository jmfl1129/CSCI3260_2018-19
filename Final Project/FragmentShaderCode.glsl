#version 440

out vec3 color;
in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 lightPositionWorld1;
uniform float diffuseBrightness;
uniform float specularBrightness;
uniform float spotBrightness;
uniform float cutoffangle;
uniform vec3 spotPositionWorld;
uniform vec3 eyePositionWorld;

uniform bool normalMapping_flag;

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler_normal;
void main()
{
	vec3 normal = normalize(normalWorld);
	if (normalMapping_flag)
	{
		normal = texture(myTextureSampler_normal, UV).rgb;
		normal = normalize(normal * 2.0 - 1.0);
	}
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normal) * diffuseBrightness;
	vec3 diffuseLight = vec3 (brightness, brightness, brightness);

	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normal);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	vec3 specularLight = vec3(s * specularBrightness, s * specularBrightness, s * specularBrightness);

	//light source 2
	vec3 lightVectorWorld1 = normalize(lightPositionWorld1 - vertexPositionWorld);
	float brightness1 = dot(lightVectorWorld1, normal) * diffuseBrightness;
	vec3 diffuseLight1 = vec3 (brightness1, brightness1, brightness1);

	vec3 reflectedLightVectorWorld1 = reflect(-lightVectorWorld1, normal);
	float s1 = clamp(dot(reflectedLightVectorWorld1, eyeVectorWorld), 0, 1);
	s1 = pow(s1, 50);
	vec3 specularLight1 = vec3(s1 * specularBrightness, s1 * specularBrightness, s1 * specularBrightness);

	//spot light
	vec3 spotVectorWorld = normalize(spotPositionWorld - vertexPositionWorld);
	float spot = dot(spotVectorWorld, normal);
	vec3 spotLight = vec3(spot * spotBrightness, spot * spotBrightness, spot * spotBrightness);
	if (spot > cutoffangle)
	{
	color = texture( myTextureSampler, UV ).rgb * (ambientLight + clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1) + clamp(spotLight, 0, 1) + clamp(diffuseLight1, 0, 1) + clamp(specularLight1, 0, 1));
	}
	else
	color = texture( myTextureSampler, UV ).rgb * (ambientLight + clamp(diffuseLight, 0, 1) + clamp(specularLight, 0, 1) + clamp(diffuseLight1, 0, 1) + clamp(specularLight1, 0, 1));
}
