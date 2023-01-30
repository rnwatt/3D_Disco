#version 420

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 texcoord;
out vec3 texCoords;

uniform mat4 model, view, projection;

void main()
{
    texCoords = texcoord;
    gl_Position = projection * view * vec4(aPos, 3.0);
}  
