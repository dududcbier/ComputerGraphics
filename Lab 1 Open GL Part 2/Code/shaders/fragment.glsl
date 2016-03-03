#version 330
// Specifying the inputs to the fragment shader
in vec3 vertexColor;

// Specifying the Uniforms of the vertex shaders
uniform vec3 materialColor;
uniform vec4 intensity;
uniform vec3 lightpos;

// Specifying the output of the fragment shader
out vec4 fColor;

void main()
{
    fColor = vec4(materialColor, 1.0);

}
