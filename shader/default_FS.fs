precision mediump	float ;
uniform sampler2D texture ;
varying vec2 outUV;
varying vec4 outColor;

void main()
{
	vec4 color = texture2D( texture,outUV );
	if(color.a <= 0.4)
		discard;
	else
		gl_FragColor =	outColor * color ;	

}