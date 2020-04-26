#version 330 core 

// =============================================================================
//		--= IN =--

// Matrices 
uniform mat4 u_normal_matrix; 
uniform mat4 u_model_matrix; 
uniform mat4 u_view_matrix; 
uniform mat4 u_projection_matrix; 

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
out vec3 FragPos;
out vec3 v_normal;
out vec2 v_uvcoord;

 
void main() { 

	// uv coords
	v_uvcoord = a_uvcoord;

	FragPos = vec3(u_model_matrix * vec4(a_position, 1.0));
    v_normal = mat3(transpose(inverse(u_model_matrix))) * a_normal;  

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

		gl_Position = (u_projection_matrix * u_view_matrix) * vec4(billboardpos, 1.0);
	}
	else {
	
		// assign position data
		gl_Position = (u_projection_matrix * u_view_matrix * u_model_matrix) * position;
	}
} 