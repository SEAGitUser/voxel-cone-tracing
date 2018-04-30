//Rafael Sabino
//based off of https://learnopengl.com/In-Practice/Text-Rendering

#version 330 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec3 textureCoord;

uniform mat4 projection;

out vec3 textCoords;


void main()
{

    gl_Position = projection * vec4(position.xyz, 1.0f);
    textCoords = textureCoord;
}
