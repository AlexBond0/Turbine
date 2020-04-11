#version 130 

// =============================================================================
//		--= OUT =--

precision highp float;

uniform sampler2D u_texture;
uniform bool u_usesTexture;
uniform bool u_usesLight; 

in vec2 v_uvcoord;
in vec4 v_colour; 

out vec4 out_color;

void main() { 

	// if we're including textures
	if (u_usesTexture) {

		vec4 textureColour = texture(u_texture, v_uvcoord);

		// does the texture include light information
		if (u_usesLight) 
			out_color=(v_colour * textureColour); 
		
		else 
			out_color=(textureColour); 

	}
	else {
	
		out_color=(v_colour); 
	}
} 