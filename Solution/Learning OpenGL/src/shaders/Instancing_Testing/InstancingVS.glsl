#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 5) in vec4 aColor;

out vec4 FragmentColor;

uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    vec2 pos = aPosition.xy;
    gl_Position = vec4(pos + offset, 0.0, 1.0);
    FragmentColor = aColor;
}  