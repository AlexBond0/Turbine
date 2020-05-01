#version 330 core

// =============================================================================
//		--= OUT =--

precision highp float;

uniform mat4 u_normal_matrix; 

// Camera
uniform vec3 u_c_position;
// uniform vec3 u_c_direction;

// Light 
uniform vec3 u_l_position; 
uniform vec3 u_l_direction; 
uniform vec3 u_l_halfplane; 
uniform vec4 u_l_ambient; 
uniform vec4 u_l_diffuse; 
uniform vec4 u_l_specular; 
uniform float u_l_spec_strength;
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


		// ambient
		float ambientStrength = 0.1;
		vec4 ambient = ambientStrength * u_l_ambient;
  	
		// diffuse 
		vec3 norm = normalize(v_normal);
		// vec3 lightDir = normalize(u_l_position - FragPos);
		vec3 lightDir = normalize(-u_l_direction);
		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = diff * u_l_diffuse;
    
		// specular
		vec3 viewDir = normalize(u_c_position - FragPos);
		vec3 reflectDir = reflect(-u_l_direction, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec4 specular = normalize(u_l_spec_strength * spec * u_l_specular);  
   
		// combine
		v_colour = (ambient * u_m_ambient) 
					+ (diffuse * u_m_diffuse)
					+ (specular * u_m_specular);

	
		//// Calculate and normalise eye space normal 
		//vec3 ecNormal=vec3(u_normal_matrix*vec4(v_normal, 0.0)); 
		//ecNormal=ecNormal/length(ecNormal); 

		//// Do light calculations 
		//float ndotl=max(0.0, dot(ecNormal, normalize(u_l_direction - FragPos))); 
		//float ndoth=max(0.0, dot(ecNormal, u_l_halfplane));

		//// Ambient light 
		//vec4 ambientLight = u_l_ambient * u_m_ambient; 

		//// Diffuse light 
		//vec4 diffuseLight = ndotl * u_l_diffuse * u_m_diffuse; 

		//// Specular light 
		//vec3 viewDir = normalize(u_c_position - FragPos);
		//vec3 reflectDir = reflect(-u_l_direction, v_normal); 

		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		//vec4 specularLight = spec * u_l_spec_strength * u_l_specular; // ; * u_m_specular;

		// vec4 specularLight=vec4(0.0); 

		// if (ndoth > 0.0)  
		// 	specularLight=pow(ndoth, u_m_shininess) * u_l_specular * u_m_specular; 


		// pass varying values to the fragment shader
		// v_colour = ambientLight + diffuseLight + specularLight; 
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