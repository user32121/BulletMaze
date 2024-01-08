#version 120

uniform sampler2D texture;

//colors corresponding to bullet values (centered at 3)
const vec4 cols[7] = {
    {0, 1, 1, 1},
    {0, 0, 1, 1},
    {0.5, 0, 1, 1},
    {1, 1, 1, 0.5},
    {1, 0, 0, 1},
    {1, 0.5, 0, 1},
    {1, 1, 0, 1},
};
uniform int colIdx = 0;

//texture coordinate data
uniform ivec2 tileSize;
uniform ivec2 textureSize;
uniform ivec2 bulletSpriteIdx;

in vec4 pos;

void main(){
    vec4 pixel = texture2D(texture, (mod(pos.xy, tileSize) + bulletSpriteIdx * tileSize) / textureSize);

    gl_FragColor = gl_Color * pixel * cols[int(clamp(colIdx+3, 0, 7))];
}
