#version 330
in vec4 fragPos;
out vec4 fragColor;

void main()
{
	fragColor = fragPos;
}