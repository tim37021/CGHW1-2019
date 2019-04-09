#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 vp;

out vec3 v_position;
out vec2 v_uv;
out vec3 v_normal;

void main()
{
    v_position = position;
    v_uv = texcoord;
    v_normal = normal;
    gl_Position = vp*model*vec4(position, 1.0);
}