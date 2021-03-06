#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

out vec4 FragColor;

void main() {
	// ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(norm, -lightDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128 * material.shininess);
    vec3 specular = vec3(0.0);
    if(diff > 0.0) {
        specular = light.specular * (spec * material.specular);  
    }
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}