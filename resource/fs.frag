#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform vec3 object_color;

in vec2 g_uv;
in vec3 g_normal;

void main()
{
    color = vec4(g_normal, 1.0);//vec4(object_color, 1.0)*texture(text, g_uv);
}