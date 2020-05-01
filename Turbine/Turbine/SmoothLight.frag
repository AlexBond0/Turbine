#version 330 core

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
uniform PointLight pointLight;

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

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() { 

	vec4 v_colour;

	if (u_usesLight) {

		vec3 norm = normalize(v_normal);
		vec3 viewDir = normalize(u_c_position - FragPos);

		v_colour = CalcPointLight(pointLight, norm, FragPos, viewDir);

		//// ambient
		//float ambientStrength = 0.1;
		//vec4 ambient = ambientStrength * pointLight.ambient;
		//ambient *= u_m_ambient;
  	
		//// diffuse
		//float diff = max(dot(norm, lightDir), 0.0);
		//vec4 diffuse = diff * pointLight.diffuse;
		//diffuse *= u_m_diffuse;
    
		//// specular
		//vec3 reflectDir = reflect(-lightDir, norm);  
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		//vec4 specular = pointLight.specStrength * spec * pointLight.specular;  
		//specular *= u_m_specular;

		//// attenuation
		//float dist = length(pointLight.position - FragPos);
		//float attenuation = 1.0 / (pointLight.constant + pointLight.linear * dist + pointLight.quadratic * (dist * dist));   
		//ambient *= attenuation;
		//diffuse *= attenuation;
		//specular *= attenuation;

		//// combine
		//v_colour = ambient + diffuse + specular;


		// v_colour = ambient + diffuse + specular;
		//v_colour = (ambient * u_m_ambient) 
		//			+ (diffuse * u_m_diffuse)
		//			+ (specular * u_m_specular);

	
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


vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

	vec3 lightDir = normalize(pointLight.position - FragPos);

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
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specular = light.specStrength * spec * light.specular;  
	specular *= u_m_specular;

	// attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));   
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// combine
	return ambient + diffuse + specular;
}
















//#version 330 core

//struct Material {

//	bool u_m_usesLight; 
//	vec4 u_m_ambient; 
//	vec4 u_m_diffuse; 
//	vec4 u_m_specular; 
//	float u_m_shininess; 

//	// texture
//	sampler2D u_texture;
//	bool u_usesTexture;
//}; 

//struct DirLight {

//    vec3 direction;
	
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//};

//struct PointLight {

//    vec3 position;
    
//    float constant;
//    float linear;
//    float quadratic;
	
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//};

//struct SpotLight {

//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
  
//    float constant;
//    float linear;
//    float quadratic;
  
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;       
//};

//#define MAX_LIGHTS 8; 

//uniform DirLight dirLight;

//uniform PointLight u_l_pointLights[MAX_LIGHTS];
//uniform int u_l_pointLight_count;

//uniform SpotLight u_l_spotLight[MAX_LIGHTS];
//uniform int u_l_spotLight_count;

//// function prototypes
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//// =============================================================================
////		--= OUT =--

//precision highp float;

//uniform mat4 u_normal_matrix; 

//// Camera
//uniform vec3 u_c_position;
//// uniform vec3 u_c_direction;

//// Light 
////uniform vec3 u_l_position; 
////uniform vec3 u_l_direction; 
////uniform vec3 u_l_halfplane; 
////uniform vec4 u_l_ambient; 
////uniform vec4 u_l_diffuse; 
////uniform vec4 u_l_specular; 
////uniform float u_l_spec_strength;

//// Material 
////uniform bool u_m_usesLight; 
////uniform vec4 u_m_ambient; 
////uniform vec4 u_m_diffuse; 
////uniform vec4 u_m_specular; 
////uniform float u_m_shininess; 

//// texture
////uniform sampler2D u_texture;
////uniform bool u_usesTexture;


//in vec3 FragPos;
//in vec3 v_normal;
//in vec2 v_uvcoord;

//out vec4 out_color;

//void main() { 

//	vec4 v_colour;

//	if (u_usesLight) {

//	    vec3 norm = normalize(Normal);
//		vec3 viewDir = normalize(viewPos - FragPos);

//		    // phase 1: directional lighting
//		vec3 result = CalcDirLight(dirLight, norm, viewDir);

//		// phase 2: point lights
//		for(int i = 0; i < NR_POINT_LIGHTS; i++)
//			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
			
//		// phase 3: spot light
//		result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    

//		v_colour = vec4(result, 1.0);

//		//// ambient
//		//float ambientStrength = 0.1;
//		//vec4 ambient = ambientStrength * u_l_ambient;
  	
//		//// diffuse 
//		//vec3 norm = normalize(v_normal);
//		//// vec3 lightDir = normalize(u_l_position - FragPos);
//		//vec3 lightDir = normalize(-u_l_direction);
//		//float diff = max(dot(norm, lightDir), 0.0);
//		//vec4 diffuse = diff * u_l_diffuse;
    
//		//// specular
//		//vec3 viewDir = normalize(u_c_position - FragPos);
//		//vec3 reflectDir = reflect(-u_l_direction, norm);  
//		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//		//vec4 specular = normalize(u_l_spec_strength * spec * u_l_specular);  
   
//		//// combine
//		//v_colour = (ambient * u_m_ambient) 
//		//			+ (diffuse * u_m_diffuse)
//		//			+ (specular * u_m_specular);

	
//		//// Calculate and normalise eye space normal 
//		//vec3 ecNormal=vec3(u_normal_matrix*vec4(v_normal, 0.0)); 
//		//ecNormal=ecNormal/length(ecNormal); 

//		//// Do light calculations 
//		//float ndotl=max(0.0, dot(ecNormal, normalize(u_l_direction - FragPos))); 
//		//float ndoth=max(0.0, dot(ecNormal, u_l_halfplane));

//		//// Ambient light 
//		//vec4 ambientLight = u_l_ambient * u_m_ambient; 

//		//// Diffuse light 
//		//vec4 diffuseLight = ndotl * u_l_diffuse * u_m_diffuse; 

//		//// Specular light 
//		//vec3 viewDir = normalize(u_c_position - FragPos);
//		//vec3 reflectDir = reflect(-u_l_direction, v_normal); 

//		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//		//vec4 specularLight = spec * u_l_spec_strength * u_l_specular; // ; * u_m_specular;

//		// vec4 specularLight=vec4(0.0); 

//		// if (ndoth > 0.0)  
//		// 	specularLight=pow(ndoth, u_m_shininess) * u_l_specular * u_m_specular; 


//		// pass varying values to the fragment shader
//		// v_colour = ambientLight + diffuseLight + specularLight; 
//	}
//	else {
	
//		// pass only ambient colour to the fragment shader
//		v_colour = u_m_ambient; 
//	}

//	// if we're including textures
//	if (u_usesTexture) {

//		vec4 textureColour = texture(u_texture, v_uvcoord);

//		// does the texture include light information
//		if (u_usesLight) 
//			out_color = (v_colour * textureColour); 
		
//		else 
//			out_color = (textureColour); 

//	}
//	else {
	
//		out_color = (v_colour); 
//	}
//} 

//// calculates the color when using a directional light.
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);

//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);

//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//    return (ambient + diffuse + specular);
//}

//// calculates the color when using a point light.
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);

//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);

//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
	
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//    ambient *= attenuation;
//    diffuse *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//}

//// calculates the color when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);

//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);

//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

//    // spotlight intensity
//    float theta = dot(lightDir, normalize(-light.direction)); 
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}