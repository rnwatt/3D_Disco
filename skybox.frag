// Minimal fragment shader

#version 420

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
//take interpolated position vector as texture's direction vector and get the value from the cubemap
	FragColor = texture(skybox, TexCoords);
}