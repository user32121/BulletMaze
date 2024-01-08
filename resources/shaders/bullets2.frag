//second phase of bullet rendering
//converts bullet count to color to be applied to pixels

#version 130

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

void main(){
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    pixel.y *= 10;

    gl_FragColor = pixel.xxxw * gl_Color * cols[int(round(clamp(pixel.y + 3, 0, 7)))];
}
