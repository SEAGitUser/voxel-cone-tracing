//Rafael Sabino

//based off of https://learnopengl.com/In-Practice/Text-Rendering

#version 330 core
in vec3 textCoords;

uniform sampler2D text;
uniform vec4 textColor;

out vec4 color;

void main()
{
    vec2 t = textCoords.xy;

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, textCoords.xy).r);
    color = textColor * sampled;

}
