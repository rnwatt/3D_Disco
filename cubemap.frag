// Vertex shader to demonstrate normal mapping
// This fragment shader was adapted from the normal mapping
// example in chapter 4 of :
// OpenGL 4.0 Shading language cookbook, second edition 

#version 420

in vec4 fcolour;		// vertex colour
in vec2 ftexcoord;
in vec3 flightdir;		// in tangent space
in vec3 fviewdir;		// in tangent space

//texture position vector from interpolated vertex attribute
in vec3 texCoords;

layout (binding=0) uniform sampler2D tex1;		// texture
//layout (binding=1) uniform sampler2D tex2;		// normal map inside a texture

out vec4 FragColor;		// Output fragment colour
uniform samplerCube cubemap; // cubemap texture sampler

void main() 
{
	 FragColor = texture(cubemap, texCoords);
}
