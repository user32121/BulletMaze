//first phase of bullet rendering
//use for each bullet to be drawn
//stores info about sprite at each pixel and bullet count

#version 130

//sprite to draw
uniform sampler2D texture;
//pixel buffer (assumed to be filled with (0,0,0,0))
//storage format is (brightness, bulletValue, 0, alpha)
uniform sampler2D target;

//bullet value
uniform int value;

in vec2 boardPos;

void main(){
    vec4 texturePix = texture2D(texture, gl_TexCoord[0].xy);
    vec4 targetPix = texture2D(target, boardPos);
    texturePix *= gl_Color;

    if(texturePix.a > 0){
        targetPix.yz *= 10;
        gl_FragColor = vec4(
            max(targetPix.x, texturePix.x),
            targetPix.y + value,
            0,
            max(targetPix.w, texturePix.w));
        gl_FragColor.yz /= 10;
    }else{
        gl_FragColor = targetPix;
    }
}
