#version 330 core

#define MAX_LIGHTS 4


// Directional Light
struct DirLight {

    vec3 direction;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	float specStrength;
};
uniform DirLight dirLight;
uniform bool u_usesDirLight; 


// Point Light
struct PointLight {

    vec3 position;
   
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	float specStrength;
};
uniform PointLight pointLights[MAX_LIGHTS];
uniform int u_pointLightCount; 


// Spot Light
struct SpotLight {

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	float specStrength;   
};
uniform SpotLight spotLights[MAX_LIGHTS];
uniform int u_spotLightCount; 

// =============================================================================
//		--= OUT =--

precision highp float;

uniform mat4 u_normal_matrix; 

// Camera
uniform vec3 u_c_position;
// uniform vec3 u_c_direction;

// Light 
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

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() { 

	vec4 v_colour;

	if (u_usesLight) {

		vec3 norm = normalize(v_normal);
		vec3 viewDir = normalize(u_c_position - FragPos);

		// directional light
		if (u_usesDirLight)
			v_colour += CalcDirLight(dirLight, norm, viewDir);
		
		// point lights
		for (int i = 0; i < u_pointLightCount; i++)
			v_colour += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

		// spot lights
		for (int i = 0; i < u_spotLightCount; i++)
			v_colour += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

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

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

	// ambient
	vec4 ambient = light.ambient * u_m_ambient;

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = light.diffuse * diff;
	diffuse *= u_m_diffuse;

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_m_shininess);
	vec4 specular = light.specular * spec;
	specular *= u_m_specular;

    // combine 
    return  ambient + diffuse + specular;
}


// calculates the color when using a point light.
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

	vec3 lightDir = normalize(light.position - FragPos);

	// ambient
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * light.ambient;
	ambient *= u_m_ambient;
  	
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * light.diffuse;
	diffuse *= u_m_diffuse;
    
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_m_shininess);
	vec4 specular = light.specStrength * spec * light.specular;  
	specular *= u_m_specular;

	// attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));   
	
	// combine
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}


// calculates the color when using a spot light.
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

	// ambient
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * light.ambient;
	ambient *= u_m_ambient;
  	
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = diff * light.diffuse;
	diffuse *= u_m_diffuse;
    
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_m_shininess);
	vec4 specular = light.specStrength * spec * light.specular;  
	specular *= u_m_specular;

	// attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 
	
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}