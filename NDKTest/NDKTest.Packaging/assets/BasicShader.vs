#version 100

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 v_texCoord;

uniform mat4 mvp;

void main()
{
	v_texCoord = texCoord;
	gl_Position = mvp * vec4(position, 1.0, 1.0);
}