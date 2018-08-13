#version 100

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 v_texCoord;

uniform mat4 u_mvp;

void main()
{
	v_texCoord = texCoord;
	gl_Position = u_mvp * vec4(position, 1.0, 1.0);
}