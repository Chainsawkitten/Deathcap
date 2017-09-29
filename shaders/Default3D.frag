/*
Geometry pass fragment shader (first pass).
*/
#version 400

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn;

uniform sampler2D mapAlbedo;
uniform sampler2D mapNormal;
uniform sampler2D mapMetallic;
uniform sampler2D mapRoughness;

layout(location = 0) out vec4 fragmentColor;


// Calculate normal based on interpolated vertex normal, sampled normal (from normal map) and vertex tangent.
vec3 calculateNormal(in vec3 normal, in vec3 tangent, in vec3 normalFromMap) {
    vec3 n = normalize(normal);
    vec3 t = normalize(tangent);
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(t, n);
    if (dot(cross(n, t), b) < 0.0)
        t = -t;
    
    vec3 mn = normalize(2.0 * normalFromMap - vec3(1.0, 1.0, 1.0));
    mat3 TBN = mat3(t, b, n);
    return TBN * mn;
}

void main() {

    vec3 color = texture(mapAlbedo, vertexIn.texCoords).rgb;
    fragmentColor = vec4(color,0);
}
