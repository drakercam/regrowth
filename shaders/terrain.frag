OF_GLSL_SHADER_HEADER

in vec3 vNormal;
in vec3 vWorldPos;

uniform vec3 viewPos;

uniform vec3 colorLow;
uniform vec3 colorMid;
uniform vec3 colorHigh;

out vec4 fragColor;

void main()
{
    float ambientStength = 0.2;
    float specularStrength = 2.0;
    float diffuseStrength = 1.2;
    vec3 lightPos = vec3(0, 420, 0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // Ambient
    vec3 ambient = ambientStength * lightColor;

    // diffuse
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - vWorldPos);
    float diff = 0.5 * (1.0 + dot(N, L));
    vec3 diffuse = diffuseStrength * diff * lightColor;

    // Specular
    vec3 V = normalize(-vWorldPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float h = vWorldPos.y;

    // Blend colors smoothly
    vec3 baseColor;
    if (h < 10.0)
        baseColor = colorLow;
    else if (h < 40.0)
    {
        // interpolate between low and mid
        float t = (h - 10.0) / (40.0 - 10.0);
        baseColor = mix(colorLow, colorMid, t);
    }
    else
    {
        // interpolate between mid and high
        float t = (h - 40.0) / 60.0; // adjust 60.0 depending on max terrain height
        baseColor = mix(colorMid, colorHigh, clamp(t, 0.0, 1.0));
    }

    vec3 result = (ambient + diffuse + specular) * baseColor.rgb;
    
    fragColor = vec4(result, 1.0);
}