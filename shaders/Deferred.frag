/*
Lighting pass fragment shader (second pass)
*/
#version 400 core

in VertexData {
    vec2 texCoords;
} vertexIn;

uniform sampler2D textureAlbedo;
uniform sampler2D textureNormal;
uniform sampler2D textureMetallic;
uniform sampler2D textureRougness;


uniform sampler2D tDepth;

uniform mat4 inverseProjectionMatrix;

struct Light {
    vec4 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;
    vec3 direction;
};

const int maxLights = 32;
uniform int lightCount;
uniform Light lights[maxLights];

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

const float PI = 3.14159265359;

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a	= roughness * roughness;
	float a2 = a*a;
	float NdotH	= max(dot(N,H), 0.0f);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;

	float denom	=(NdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return clamp((nom /denom),0,5);
}

float GemetrySchlickGGX(float NdotV, float roughness)
{
	float r	= (roughness + 1.0f);
	float k	= (r*r) / 8.0f;

	float nom = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return clamp((nom/denom),0,1);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx1 = GemetrySchlickGGX(NdotV, roughness);
	float ggx2 = GemetrySchlickGGX(NdotL, roughness);

	return clamp((ggx1 * ggx2),0,1);
}

vec3 ReconstructPos(in vec2 texCoord, in float depth)
{
    vec4 sPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    sPos = inverseProjectionMatrix * sPos;

    return (sPos.xyz / sPos.w);
}

	float depth = texture(tDepth, vertexIn.texCoords).r;
	vec3 test = texture(textureAlbedo, vertexIn.texCoords).rgb;
	vec3 albedo	= pow(test, vec3(2));
	vec3 normal	= normalize(texture(textureNormal, vertexIn.texCoords).rgb);
	float metallic = texture(textureMetallic, vertexIn.texCoords).r;
	float roughness = texture(textureRougness, vertexIn.texCoords).r;
	vec3 pos = ReconstructPos(vertexIn.texCoords, depth);
	vec3 N = normalize(normal);

vec3 applyLights()
 {
    vec3 Lo = vec3(0.0);
	
	for(int i =0; i<lightCount;i++)
		{
			vec3 V = normalize(-pos);
			vec3 surfaceToLight;
			float attenuation;

			if (lights[i].position.w == 0.0) 
			{
				//Directional light.
				surfaceToLight = normalize(lights[i].position.xyz);
				attenuation = 1.0;
			}
			else 
			{
				// Point light
				vec3 toLight = lights[i].position.xyz - pos;
				surfaceToLight = normalize(toLight);
				attenuation = 1.0 / (1.0 + lights[i].attenuation * (toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z));

				// Spot light.
				float lightToSurfaceAngle = degrees(acos(clamp(dot(-surfaceToLight, normalize(lights[i].direction)),-1,1)));
				if (lightToSurfaceAngle > lights[i].coneAngle)
				{
					attenuation = 0.0;
				}
			}

			vec3 H = normalize(V + surfaceToLight);

			vec3 radiance = vec3(1.0,1.0,.9) * attenuation; //vec3 light color 
			
			vec3 F0	= mix(vec3(0.04), albedo, metallic);
			float NDF = DistributionGGX(N,H, roughness);
			float G	= GeometrySmith(N,V,surfaceToLight,roughness);
			vec3 F = FresnelSchlick(max(dot(H,V),0.0f),F0);
			
			vec3 nominator = NDF * G * F;
			float denominator = 4 * max(dot(N,V), 0.0f) * max(dot(N,surfaceToLight), 0.0f) + 0.001;
			vec3 specular = (nominator/denominator);
			
			vec3 kS = F;
			vec3 kD = vec3(1.0f) - kS;
			kD *= 1.0f - metallic;
			
			float NdotL = max(dot(N,surfaceToLight), 0.0f);
			Lo  += (kD * albedo / PI + specular) * radiance * NdotL;
			Lo += lights[i].ambientCoefficient * albedo;
		}

	return Lo;
}


void main()
{

	
	vec3 color = applyLights();


	color = clamp(color / (color + vec3(1.0f)),0,1);
	//color = pow(color, vec3(1.0/2.2)); // Gamma correct comming soon to a game near you!

	fragmentColor = vec4(color, 1.0);
}


