#version 150

uniform sampler2D texture0;
in vec2 vTexcoord;
in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    float ambientStength = 0.2;
    float specularStrength = 2.0;
    float diffuseStrength = 1.2;
    vec3 lightPos = vec3(0, 420, 0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec4 color = texture(texture0, vTexcoord);

    // Ambient
    vec3 ambient = ambientStength * lightColor * color.rgb;

    // Diffuse
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - vFragPos);
    float diff = 0.5 * (1.0 + dot(N, L));
    vec3 diffuse = diffuseStrength * diff * lightColor * color.rgb;

    // Specular
    vec3 V = normalize(-vFragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * color.rgb;

    vec3 illuminatedColor = (ambient + diffuse + specular);
    FragColor = vec4(illuminatedColor, 1.0);
}