#version 330 core

out vec4 FragColor;

in vec2 TextureCoords;
in vec3 Normal;
in vec3 FragPosition;

float near = 0.1;
float far = 100;

void main(){
	FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}