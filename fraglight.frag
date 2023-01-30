// Phong shading fragment shader
// This is a solution to lab4
// Iain Martin 2018

#version 400

// Global constants (for this vertex shader)
vec4 specular_colour = vec4(1.0, 0.8, 0.6, 1.0);
vec4 global_ambient = vec4(0.05, 0.05, 0.05, 1.0);
int  shininess = 8;

// Inputs from the vertex shader
in vec3 fnormal, flightdir, fposition;
in vec4 fdiffusecolour, fambientcolour;

uniform uint attenuationmode;
uniform uint emitmode;

// Output pixel fragment colour
out vec4 outputColor;
void main()
{
	// Create a vec4(0, 0, 0) for our emmissive light but set to zero unless the emitmode flag is set
	vec4 emissive = vec4(0);				
	vec4 fambientcolour = fdiffusecolour * 0.2;
	vec4 fspecularcolour =  vec4(1.0, 0.8, 0.6, 1.0);
	float distancetolight = length(flightdir);

	// Normalise interpolated vectors
	vec3 L = normalize(flightdir);
	vec3 N = normalize(fnormal);		

	// Calculate the diffuse component
	vec4 diffuse = max(dot(N, L), 0.0) * fdiffusecolour;

	// Calculate the specular component using Phong specular reflection
	vec3 V = normalize(-fposition);	
	vec3 R = reflect(-L, N);
	vec4 specular = pow(max(dot(R, V), 0.0), shininess) * fspecularcolour;

	// Calculate the attenuation factor;
	// Turn off attenuation if attenuationmode is not set to 1 (on)
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
		attenuation = 1.0 / (attenuation_k1 + attenuation_k2*distancetolight + 
								   attenuation_k3 * pow(distancetolight, 2));
	}
	
	// If emitmode is 1 then we enable emmissive lighting
	if (emitmode == 1) emissive = vec4(1.0, 1.0, 0.8, 1.0); 

	// Calculate the output colour, includung attenuation on the diffuse and specular components
	// Note that you may want to exclude the ambient from the attenuation factor so objects
	// are always visible, or include a global ambient
	outputColor = attenuation*(fambientcolour + diffuse + specular) + emissive + global_ambient;
	
}