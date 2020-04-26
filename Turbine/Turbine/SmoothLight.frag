#version 330 core

// =============================================================================
//		--= OUT =--

precision highp float;

uniform mat4 u_normal_matrix; 

// Light 
uniform vec3 u_l_direction; 
uniform vec3 u_l_halfplane; 
uniform vec4 u_l_ambient; 
uniform vec4 u_l_diffuse; 
uniform vec4 u_l_specular; 
uniform bool u_usesLight; 

// Material 
uniform vec4 u_m_ambient; 
uniform vec4 u_m_diffuse; 
uniform vec4 u_m_specular; 
uniform float u_m_shininess; 

// texture
uniform sampler2D u_texture;
uniform bool u_usesTexture;

in vec3 FragPos;
in vec3 v_normal;
in vec2 v_uvcoord;

out vec4 out_color;

void main() { 

	vec4 v_colour;

	if (u_usesLight) {
	
		// Calculate and normalise eye space normal 
		vec3 ecNormal=vec3(u_normal_matrix*vec4(v_normal, 0.0)); 
		ecNormal=ecNormal/length(ecNormal); 

		// Do light calculations 
		float ndotl=max(0.0, dot(ecNormal, normalize(u_l_direction - FragPos))); 
		float ndoth=max(0.0, dot(ecNormal, u_l_halfplane));

		// Ambient light 
		vec4 ambientLight = u_l_ambient * u_m_ambient; 

		// Diffuse light 
		vec4 diffuseLight = ndotl * u_l_diffuse * u_m_diffuse; 

		// Specular light 
		vec4 specularLight=vec4(0.0); 

		if (ndoth > 0.0)  
			specularLight=pow(ndoth, u_m_shininess) * u_l_specular * u_m_specular; 


		// pass varying values to the fragment shader
		v_colour = ambientLight + diffuseLight + specularLight; 
	}
	else {
	
		// pass only ambient colour to the fragment shader
		v_colour = u_m_ambient; 
	}

	// if we're including textures
	if (u_usesTexture) {

		vec4 textureColour = texture(u_texture, v_uvcoord);

		// does the texture include light information
		if (u_usesLight) 
			out_color = (v_colour * textureColour); 
		
		else 
			out_color = (textureColour); 

	}
	else {
	
		out_color = (v_colour); 
	}
} 