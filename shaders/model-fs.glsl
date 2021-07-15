#version 330 core

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

out vec4 FragColor;

in vec2 TexPos;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 cameraPos;

vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 norm);

void main() {
    vec3 viewDir = normalize(FragPos - cameraPos);
    vec3 norm = normalize(Normal);
    
    vec3 result = CalcDirLight(dirLight, viewDir, norm);
    
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
