#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


void main(void)
{
    int i = 0;
    int points = 0;

    for ( i = 0; i < gl_in.length(); i++)
    {
		gl_Position = gl_in[i].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(0.0, 500.0, 0.0, 0.0);
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(500.0, 0.0, 0.0, 0.0);
        EmitVertex();

		gl_Position = gl_in[i].gl_Position + vec4(500.0, 500.0, 0.0, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}