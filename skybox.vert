#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout (location = 3) in vec3 aPos;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
uniform uint colourmode;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;
out vec3 normal_mv;
out vec3 TexCoords;

//vertex position, normal, light direction and colour

uniform uint attenuationmode;



void main()
{
//normalised device coordinates
//z value will be equal to 1.0
	TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

