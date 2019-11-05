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
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outTrace;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

//The light source is attached to camera

void main() 
{
	vec3 world_position = vec3(ubo.viewMatrix * ubo.modelMatrix * vec4(inPos.xyz, 1.0));
	gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPos.xyz, 1.0);
	vec3 light_pos = vec3(0.0, 0.0, 0.0);
	vec3 trace = world_position - light_pos;
	vec3 norm_trace = normalize(trace);
	vec3 normal = normalize(mat3(inverse(transpose(ubo.viewMatrix * ubo.modelMatrix))) * inNormal);

	// As closer the normal vector is to trace and as closer to light_source is vert as brighter it will be

//	float enlighted = max(abs(dot(normal, norm_trace))  * (1000.0 - length(trace)) / 1000.00 , 0.0);

	outNormal = normal;
	outTrace = trace;

//	outColor = inColor * (enlighted * 0.7 + 0.1);
	outColor = inColor;
}
