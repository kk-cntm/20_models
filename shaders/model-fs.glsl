#version 330 core

#define SPOT_LIGHTS_NR 1

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float innerCone;
    float outerCone;
    
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexPos;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLights[SPOT_LIGHTS_NR];
uniform vec3 cameraPos;

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 norm);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir, vec3 norm);

void main() {
    vec3 viewDir = normalize(FragPos - cameraPos);
    vec3 norm = normalize(Normal);
    
    vec3 result = CalcDirLight(dirLight, viewDir, norm);
    
    for (int i = 0; i < SPOT_LIGHTS_NR; i++) {
        result += CalcSpotLight(spotLights[i], viewDir, norm);
    }
    
    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 norm) {
    vec3 normDir = normalize(light.direction);
    vec3 diffColor = texture(material.texture_diffuse1, TexPos).rgb;
    
    vec3 ambient = light.ambient * diffColor;
    
    float diffCos = max(dot(-normDir, norm), 0.0f);
    vec3 diffuse = light.diffuse * diffCos * diffColor;
    
    vec3 reflection = reflect(normDir, norm);
    float spec = pow(max(dot(reflection, -viewDir), 0.0f), 2.0f);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexPos).rgb;
    
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 viewDir, vec3 norm) {
    vec3 normDir = normalize(light.direction);
    vec3 diffColor = texture(material.texture_diffuse1, TexPos).rgb;
    
    vec3 ambient = light.ambient * diffColor;
    
    float diffCos = max(dot(-normDir, norm), 0.0f);
    vec3 diffuse = light.diffuse * diffCos * diffColor;
    
    vec3 reflection = reflect(normDir, norm);
    float spec = pow(max(dot(reflection, -viewDir), 0.0f), 2.0f);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexPos).rgb;
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    vec3 spotDir = normalize(FragPos - light.position);
    float theta = clamp(dot(spotDir, light.direction), 0.0f, 1.0f);
    float epsilon = light.innerCone - light.outerCone;
    float intensity = (theta - light.outerCone) / epsilon;
    
    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;
    
    return ambient + diffuse + specular;
}
