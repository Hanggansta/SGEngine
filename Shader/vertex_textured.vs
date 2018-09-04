#version 450 core

//Input from the program
layout (location = 0) in vec3 lPos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

//Output to fragment stage
out vec4 vs_color;
out vec2 vs_texCoord;

void main()
{
    gl_Position = vec4(lPos.x,lPos.y,lPos.z,1.0);
    vs_color = color;
	vs_texCoord = texCoord;
}