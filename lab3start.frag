// Minimal fragment shader

#version 420
in vec4 fcolour;
in vec4 position_h;
in mat3 n_matrix;
in	vec3 normal_mv;
in vec3 lightPos;
in vec4 diffuse_colour;
out vec4 outputColor;
in vec2 ftexcoord;

uniform sampler2D tex1;	
void main()
{
// Extract the texture colour to colour our pixel
	vec4 texcolour = texture(tex1, ftexcoord);

	// Set the pixel colour to be a combination of our lit colour and the texture
	outputColor = fcolour * texcolour;
}