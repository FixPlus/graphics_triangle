#version 430

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
} ubo;

layout (location = 0) out vec3 outColor;
\
out gl_PerVertex 
{
    vec4 gl_Position;   
};


void main() 
{
	vec3 world_position = vec3(ubo.viewMatrix * ubo.modelMatrix * vec4(inPos.xyz, 1.0));
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPos.xyz, 1.0);
	vec3 light_pos = vec3(1.0, 1.0, 0.0);
	vec3 trace = world_position - light_pos;
	vec3 norm_trace = normalize(trace);
	vec3 normal = normalize(mat3(inverse(transpose(ubo.viewMatrix * ubo.modelMatrix))) * inNormal);
//	float enlighted = max(dot(normal, trace), 0.0);
	float enlighted = max(abs(dot(normal, norm_trace)) * (200.0 - length(trace)) / 200.00, 0.0);
//	outColor = normal;


	outColor = inColor * (enlighted * 0.6 + 0.2);
}
