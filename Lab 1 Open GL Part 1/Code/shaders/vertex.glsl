#version 330
// Specifing the input locations of attributes
layout(location=0) in vec3 posAttr;     // vertex position
layout(location=1) in vec3 colAttr;     // vertex color color
layout(location=2) in vec3 normalAttr;  // vertex normal

// Specifing the Uniforms of the vertex shaders
uniform mat4 m;
uniform mat4 p;
uniform mat4 v;

// Specifing the outputs of the vertex shader
// These will be the input for the fragment shader
out vec3 vertexColor;

void main()
{
    // gl_Position is the output of the vertex shader
    vertexColor=colAttr;
    gl_Position = vec4(posAttr,1.0);
    gl_Position = p * v * m * gl_Position;
}
