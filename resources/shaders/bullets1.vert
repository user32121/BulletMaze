#version 130

uniform vec2 boardSize;  //in pixels

out vec2 boardPos;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
    boardPos = gl_Vertex.xy / boardSize;
    boardPos.y = 1 - boardPos.y;
}