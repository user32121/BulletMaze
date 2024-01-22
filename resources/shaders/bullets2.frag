//second phase of bullet rendering
//converts bullet count to color to be applied to pixels

#version 130

uniform sampler2D texture;

//https://gist.github.com/983/e170a24ae8eba2cd174f
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    pixel.y = pixel.y * 255 - 127;

    //still would prefer some colouring based on total alpha rather than bullet value
    gl_FragColor = gl_Color * pixel.xxxw * vec4(hsv2rgb(vec3(clamp(pixel.y, -4, 4) / 10 - 0.12, 1, 1)), clamp(abs(pixel.y), 0.5, 1));
}
