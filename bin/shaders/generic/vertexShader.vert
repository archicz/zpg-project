#version 330
layout (location = 0) in vec3 vp;
uniform mat4 modelMat;

void main ()
{
	gl_Position = modelMat * vec4(vp, 1.0);
}