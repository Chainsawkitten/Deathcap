#version 420

//Just saying it again :) position.w holds the particle speed.
in layout (location = 0) vec4 position; 

uniform mat4 ViewMtx;
uniform mat4 ProjMtx;


void main()
{ 
	gl_Position = ProjMtx * ViewMtx * vec4(position.xyz, 1.0);
}
