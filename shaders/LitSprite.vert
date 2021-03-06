#version 420 core
layout(location = 0) in vec2 aPos;

#include <common/SceneData.glsli>

layout (std140, binding = 1) uniform ObjectData
{
	mat4  u_Model;
	vec4  u_Tint;
	float u_BaseLight;
	float u_MaterialConstantCoefficient;
	float u_MaterialLinearCoefficient;
	float u_MaterialQuadraticCoefficient;
};

layout(location = 0) out vec2 FragPos;

void main() 
{
	vec4 pos = u_Model * vec4(aPos.x,aPos.y, 0.0, 1.0);
	FragPos = pos.xy;
	gl_Position = u_ViewProjection * u_Model * vec4(aPos.xy, 0.0, 1.0);
}