#version 430

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	vec4 lightDirection;
	vec4 viewPos;
} ubo;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outTrace;
layout (location = 3) out vec3 outViewTrace;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

//The light source is attached to camera

void main() 
{
	gl_Position = ubo.projectionMatrix * vec4(inPos.xyz, 1.0);

	vec3 trace = vec3(ubo.lightDirection);
//	vec3 norm_trace = normalize(trace);
	vec3 normal = normalize(inNormal);


	outNormal = normalize(inNormal);
	outTrace = trace;
	outViewTrace = ubo.viewPos.xyz - inPos.xyz;
	outColor = inColor;
}
