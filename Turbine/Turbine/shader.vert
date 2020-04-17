#version 330 core 

// =============================================================================
//		--= IN =--

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

// Matrices 
uniform mat4 u_normalmatrix; 
uniform mat4 u_mvpmatrix; 
uniform mat4 u_vpmatrix; 

// Attributes 
layout (location = 0) in vec3 a_position; 
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uvcoord;

// instancing
uniform bool u_instancing; 
layout (location = 3) in vec3 a_offset; 

// billboarding
uniform bool u_billboarding; 
uniform vec3 a_b_up; 
uniform vec3 a_b_right; 

// =============================================================================
//		--= OUT =--

// Varyings 
out vec4 v_colour; 
out vec2 v_uvcoord;

 
void main() { 

	if (u_usesLight) {
	
		// Calculate and normalise eye space normal 
		vec3 ecNormal=vec3(u_normalmatrix*vec4(a_normal, 0.0)); 
		ecNormal=ecNormal/length(ecNormal); 

		// Do light calculations 
		float ndotl=max(0.0, dot(ecNormal, u_l_direction)); 
		float ndoth=max(0.0, dot(ecNormal, u_l_halfplane));

		// Ambient light 
		vec4 ambientLight=u_l_ambient*u_m_ambient; 

		// Diffuse light 
		vec4 diffuseLight=ndotl*u_l_diffuse*u_m_diffuse; 

		// Specular light 
		vec4 specularLight=vec4(0.0); 

		if (ndoth>0.0)  
			specularLight=pow(ndoth, u_m_shininess)*u_l_specular*u_m_specular; 


		// pass varying values to the fragment shader
		v_colour=ambientLight+diffuseLight+specularLight; 
	}
	else {
	
		// pass only ambient colour to the fragment shader
		v_colour=u_m_ambient; 
	}

	v_uvcoord = a_uvcoord;



	// handle instancing
	vec4 position;
	if (u_instancing) {

		position = vec4(
			a_offset.x + a_position.x,
			a_offset.y + a_position.y,
			a_offset.z + a_position.z,
			1.0
		);
	}
	else {
	
		position = vec4(a_position, 1.0);
	}


	// handle billboarding
	if (u_billboarding) {
	
		vec3 billboardpos =
			a_offset
			+ a_b_right * a_position.x
			+ a_b_up * a_position.y;

		gl_Position = u_vpmatrix * vec4(billboardpos, 1.0);
	}
	else {
	
		// assign position data
		gl_Position = u_mvpmatrix*position;
	}
} 