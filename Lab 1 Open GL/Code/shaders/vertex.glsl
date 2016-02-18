#version 330
// Specify the input locations of attributes
layout(location=0) in vec3 posAttr;     // vertex position
layout(location=1) in vec3 colAttr;     // vertex color color
layout(location=2) in vec3 normalAttr;  // vertex normal

// Specify the Uniforms of the vertex shaders

// Specify the outputs of the vertex shader
// These will be the input for the fragment shader


void main()
{
    // gl_Position is the output of the vertex shader
    // Currently without any transformation
    gl_Position = vec4(posAttr,1.0);
}
