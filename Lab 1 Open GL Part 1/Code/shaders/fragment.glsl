#version 330
// Specifying the inputs to the fragment shader
in vec3 vertexColor;

// Specifing the Uniforms of the vertex shaders
uniform mat4 colors;

// Specifing the output of the fragment shader
out vec4 fColor;

void main()
{
    fColor = vec4(vertexColor,1);
}
