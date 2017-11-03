#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 UV;

uniform mat4 viewProjectionMatrix;

uniform float textureIndex;
uniform float textureAtlasRows;

void main(void)
{
	float atlasColumn = mod(textureIndex, textureAtlasRows);
    float atlasRow = floor(textureIndex/textureAtlasRows);
	
	float atlasIndexOffset = 1.0/textureAtlasRows;
    vec2 atlasCoordOffset = vec2(atlasColumn*atlasIndexOffset, atlasRow*atlasIndexOffset);

    int i = 0;
    int points = 0;

	gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(0.0, 0.0, 0.0, 1.0));
	UV = vec2(0,0.25);
	EmitVertex();

	gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(0.0, 1.0, 0.0, 1.0));
	UV = vec2(0,0);
	EmitVertex();

	gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(1.0, 0.0, 0.0, 1.0));
	UV = vec2(0.25,0.25);
	EmitVertex();

	gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(1.0, 1.0, 0.0, 1.0));
	UV = vec2(0.25,0);
	EmitVertex();

	EndPrimitive();
}