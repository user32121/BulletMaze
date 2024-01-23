//first phase of bullet rendering
//use for each bullet to be drawn
//stores info about sprite at each pixel and bullet count

#version 130

//sprite to draw
uniform sampler2D texture;
//pixel buffer (assumed to be filled with (0,127,0,0))
//storage format is (brightness, bulletValue[1], undefined, bulletValue[0])
uniform sampler2D target;

//bullet value
uniform int value;

in vec2 boardPos;

void main(){
    vec4 texturePix = texture2D(texture, gl_TexCoord[0].xy);
    vec4 targetPix = texture2D(target, boardPos);
    texturePix *= gl_Color;

    gl_FragColor.x = targetPix.x + texturePix.x;

    //merge y and w (treated similar to concatenated ints)
    gl_FragColor.y = targetPix.y * 255 - 127 + targetPix.w;
    //add value
    gl_FragColor.y += texturePix.w * value;
    //convert back
    gl_FragColor.w = fract(gl_FragColor.y);
    gl_FragColor.y = (floor(gl_FragColor.y) + 127) / 255;
}
