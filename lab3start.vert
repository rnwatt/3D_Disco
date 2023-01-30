// Starter vertex shader for lab3
// THe goal is to update this shader to implement Gourand shading
// which is per-vertex lighting

#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoord;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
uniform uint colourmode;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;
out vec3 normal_mv;
// Output the  texture coordinate - just pass it through
out vec2 ftexcoord;
//vertex position, normal, light direction and colour

uniform uint attenuationmode;



void main()
{
	vec3 lightPos = normalize(vec3(1,1,1));
	vec4 position_h = vec4(position, 1.0);
	mat4 model_view = view * model;
	mat3 n_matrix = transpose(inverse(mat3(model_view)));
	vec3 normal_mv = normalize(n_matrix * normal);
	vec4 diffuse_colour;
	float distanceToLight = length(lightPos);	// For attenuation
	float diffuseLight = dot(normal_mv, lightPos);

	//1.Define the vertex position transformed by the model-view transformation.
	vec4 vertex_mv = model_view * position_h;

	//2.Define the shininess component by setting the value of a float variable, e.g. 8.0;
	float specularShine = 8.0;
	vec4 specColour = vec4(1,1,1,1);

	//Specular Reflection
	vec3 V = normalize(-vertex_mv.xyz);
	vec3 R = reflect(-lightPos, normal_mv);
	vec4 specular = pow(max(dot(R, V), 0.0), specularShine) * specColour;

	// Calculate the attenuation factor;
	float attenuation;
	if (attenuationmode != 1)
	{
		attenuation = 1.0;
	}
	else
	{
		// Define attenuation constants. These could be uniforms for greater flexibility
		float attenuation_k1 = 0.5;
		float attenuation_k2 = 0.5;
		float attenuation_k3 = 0.5;
		attenuation = 1.0 / (attenuation_k1 + attenuation_k2*distanceToLight + 
								   attenuation_k3 * pow(distanceToLight, 2));
	}

	//Emissive Lighting
	vec4 emissiveLight = vec4(1.0,1.0,1.0,1.0);

	vec4 diffuseColour = diffuseLight * colour;
	vec4 ambient = colour * 0.2;
	// Define the vertex colour
	fcolour = vec4(attenuation*(ambient + diffuseColour + specular));
	// add emissiveLight to fcolour to add light.

	// Define the vertex position
	gl_Position = projection * view * model * position_h;

	// Pas through the texture coordinate
	ftexcoord = texcoord;
}

