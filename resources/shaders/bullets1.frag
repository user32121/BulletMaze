//first phase of bullet rendering
//use for each bullet to be drawn
//stores info about sprite at each pixel and bullet count

#version 130

//sprite to draw
uniform sampler2D texture;
//pixel buffer (assumed to be filled with (0,127,0,0))
//storage format is (brightness, bulletValue, undefined, alpha)
uniform sampler2D target;

//bullet value
uniform int value;

in vec2 boardPos;

void main(){
    vec4 texturePix = texture2D(texture, gl_TexCoord[0].xy);
    vec4 targetPix = texture2D(target, boardPos);
    texturePix *= gl_Color;

    if(texturePix.a > 0){
        gl_FragColor.ra = max(targetPix.ra, texturePix.ra);

        targetPix.y = targetPix.y * 255 - 127;
        gl_FragColor.y = targetPix.y + value;
        gl_FragColor.y = (gl_FragColor.y + 127) / 255;
    }else{
        gl_FragColor = targetPix;
    }
}
