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
    pixel.yz *= 10;

    gl_FragColor = pixel.xxxw * gl_Color * cols[int(round(clamp(pixel.y + 3, 0, 7)))];
    
    /*if(pixel.z > 0 && pixel.z < 0.3){
        gl_FragColor = vec4(1,0,0,1);
    }
    if(pixel.z > 0.3 && pixel.z < 0.5){
        gl_FragColor = vec4(1,0,1,1);
    }
    if(pixel.z > 0.5 && pixel.z < 0.7){
        gl_FragColor = vec4(0,0,1,1);
    }
    if(pixel.z > 0.7 && pixel.z < 0.9){
        gl_FragColor = vec4(0,1,1,1);
    }
    if(pixel.z > 0.9 && pixel.z < 1){
        gl_FragColor = vec4(0,1,0,1);
    }*/
}
