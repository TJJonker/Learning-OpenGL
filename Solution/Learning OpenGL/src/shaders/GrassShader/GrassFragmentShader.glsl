#version 330 core

out vec4 FragColor;

in vec2 TextureCoords;
in vec3 Normal;
in vec3 FragPosition;

uniform sampler2D texture_diffuse1;

void main(){
	//FragColor = texture(texture_diffuse1, TextureCoords);
	vec4 textureColor = texture(texture_diffuse1, TextureCoords);
	if(textureColor.a < 0.1)
		discard;
	FragColor = textureColor;
}