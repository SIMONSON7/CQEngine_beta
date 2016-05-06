uniform mat4 MVP;
attribute vec3 vPosition ;
attribute vec2 vUV;
attribute vec4    vColor;
varying vec2 outUV;
varying vec4 outColor;
void main()
{
    gl_Position = MVP * vec4(vPosition,1);
    outUV = vUV;
    outColor = vColor ;
}