#version 330
// Specify the inputs to the fragment shader
in vec2 texCoord;

// Specify the Uniforms of the vertex shaders
uniform sampler2D textureColor;

// Specify the output of the fragment shader
// Usually a vec4 describing a color
out vec4 fColor;

void main()
{
    // Plain white

    fColor = texture2D(textureColor, texCoord);
    //fColor = texture2D(textureColor, vec2(0.5, 0.5));
}
