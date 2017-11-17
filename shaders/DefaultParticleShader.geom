#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 UV;
in vec4 myColor[];
out vec4 finalColor;
uniform mat4 viewProjectionMatrix;
uniform int textureIndex;
uniform float textureAtlasRows;
uniform float scale;

void main(void) {
    float atlasColumn = mod(textureIndex, textureAtlasRows);
    float atlasRow = floor(textureIndex/textureAtlasRows);
    
    float atlasIndexOffset = 1.0/textureAtlasRows;
    vec2 atlasCoordOffset = vec2(atlasColumn*atlasIndexOffset, atlasRow*atlasIndexOffset);
    
    int i = 0;
    int points = 0;
    
    finalColor = myColor[i];
    
    gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(-scale - 0.9, -scale - 0.9, 0.0, 0.0));
    UV = vec2(atlasCoordOffset.x, atlasCoordOffset.y + atlasIndexOffset);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(-scale - 0.9, scale + 0.9, 0.0, 0.0));
    UV = vec2(atlasCoordOffset.x, atlasCoordOffset.y);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(scale + 0.9, -scale - 0.9, 0.0, 0.0));
    UV = vec2(atlasCoordOffset.x+atlasIndexOffset, atlasCoordOffset.y+atlasIndexOffset);
    EmitVertex();
    
    gl_Position = viewProjectionMatrix * (gl_in[i].gl_Position + vec4(scale + 0.9, scale + 0.9, 0.0, 0.0));
    UV = vec2(atlasCoordOffset.x + atlasIndexOffset, atlasCoordOffset.y);
    EmitVertex();
    
    EndPrimitive();
}