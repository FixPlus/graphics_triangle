#version 430

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inTrace;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  vec3 norm_trace = normalize(inTrace);
  float enlighted = max(abs(dot(inNormal, norm_trace)) * (200.0 - length(inTrace)) / 200.00, 0.0);
  outFragColor = vec4(inColor * (enlighted * 0.7 + 0.1), 1.0);
}
