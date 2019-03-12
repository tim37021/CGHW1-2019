#version 330
layout(location=0) out vec4 color;

uniform sampler2D text;
uniform vec3 object_color;

in vec2 uv;

void main()
{
    color = vec4(object_color, 1.0)*texture(text, uv);
}