#version 130

uniform sampler2D texture;
uniform sampler2D texture2;

void main(){
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 pixel2 = texture2D(texture2, gl_TexCoord[0].xy);
    pixel.rg = mod(pixel.rg + pixel2.rg, 1);
    gl_FragColor = pixel;
}
