#version 150

in vec3 position;
in vec2 texcoord;
in vec3 normal;

out vec2 vTexcoord;
out vec3 vNormal;
out vec3 vFragPos;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);
    vFragPos = worldPos.xyz;
    mat3 normal_mat3 = transpose(inverse(mat3(model)));
    vNormal = normalize(normal_mat3 * normal);
    vTexcoord = texcoord;
    gl_Position = mvp * worldPos;
}