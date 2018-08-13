#version 100

precision mediump float;

varying vec2 v_texCoord;

uniform sampler2D tex;

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);//texture2D(tex, v_texCoord);
}