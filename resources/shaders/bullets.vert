#version 110

out vec4 pos;

void main()
{
    // default behavior
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    
    //transfer world coordinates
    pos = gl_Vertex;
}
