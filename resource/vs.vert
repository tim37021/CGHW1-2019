#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;

out vec2 uv;

void main()
{
    uv = texcoord;
    gl_Position = vp*model*vec4(position, 1.0);
}