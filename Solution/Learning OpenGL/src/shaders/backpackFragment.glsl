#version 330 core

out vec4 FragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TextureCoords;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform DirectionalLight directionalLight;

float shininess;
vec3 viewPosition;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
    // Calculate the inverse light direction.
    vec3 lightDirection = normalize(-light.direction);
    // Calculate the direction of the reflection.
    vec3 reflectDirection = reflect(-lightDirection, normal);


    vec3 ambientResult = light.ambient * vec3(texture(texture_diffuse1, TextureCoords));

    float diffuse = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseResult = light.diffuse * diffuse * vec3(texture(texture_diffuse1, TextureCoords));
    
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specularResult = light.specular * specular * vec3(texture(texture_specular1, TextureCoords));
    return (ambientResult + diffuseResult + specularResult);
}

void main()
{   
    vec3 fragmentNormal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragPosition);

    vec3 result = CalculateDirectionalLight(directionalLight, fragmentNormal, viewDirection);

    FragColor = vec4(result, 1.0f);
}