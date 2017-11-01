#version 400

//Just saying it again :) position.w holds the particle speed.
layout (location = 0) in vec4 position; 

uniform mat4 viewProjectionMatrix;

void main()
{ 
	gl_Position = viewProjectionMatrix * vec4(position.xyz, 1.0);
}
