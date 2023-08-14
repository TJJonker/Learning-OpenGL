#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINTS_LIGHT 4
uniform PointLight pointLights[NR_POINTS_LIGHT];

uniform DirLight dirLight;
uniform Material material;
  
uniform vec3 viewPosition;

in vec2 TextureCoords;
in vec3 Normal;
in vec3 FragPos;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation shading
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));

    return (ambient + diffuse + specular) * attenuation;
}

void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINTS_LIGHT; i++){
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    //result += someFunctionToCalculateSpotLight();
    FragColor = vec4(result, 1.0);
}



//void main()
//{
//    // Ambient
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));
//
//    // Diffuse
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(light.position - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
//
//    // Specuar 
//    vec3 viewDir = normalize(viewPosition - FragPos); 
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));
//
//    float theta = dot(lightDir, normalize(-light.direction));   
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//
//    diffuse *= intensity;
//    specular *= intensity;
//
//    vec3 result = ambient + diffuse + specular;
//    FragColor = vec4(result, 1.0);
//
//
//}