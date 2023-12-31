#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoords;
out vec3 Normal;
out vec3 FragPosition;

void main(){
	TextureCoords = aTextureCoords;	
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPosition = vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}