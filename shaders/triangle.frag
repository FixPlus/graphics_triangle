#version 430

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inTrace;
layout (location = 3) in vec3 inViewTrace;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  vec3 norm_trace = normalize(inTrace);
  vec3 norm_view_trace = normalize(inViewTrace);
  vec3 reflect = normalize(reflect(norm_trace, inNormal));
  float enlighted = max((dot(inNormal, -norm_trace) + 1.0) * 0.5 *(1000.0 - length(inViewTrace)) / 1000.00, 0.0);
//  float enlighted = max((dot(inNormal, -norm_trace) + 1.0) * 0.5, 0.0);
//  enlighted = enlighted *((abs(dot(reflect, norm_view_trace) + 4.0) / 5.0));
//  vec3 specular = pow(max(dot(reflect, norm_view_trace), 0.0), 16) * vec3(1.0);
	float specular = max(pow(max(dot(reflect, norm_view_trace), 0.0), 16) * (200.0 - length(inViewTrace)) / 200.00, 0.0);
  
  if(dot(inNormal, -norm_trace) < 0)
  	specular *= 0;
  outFragColor = vec4(inColor * (enlighted * 0.7 + 0.1) + specular, 1.0);
  //outFragColor = vec4(specular, 1.0);
}
