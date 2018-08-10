#version 120

varying vec2 v_texCoord;

uniform sampler2D tex;

void main()
{
	gl_FragColor = texture(tex, v_texCoord);
}