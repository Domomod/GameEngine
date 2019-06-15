#version 420 core

layout( location = 0 ) out vec4 fragColor;

layout (std140, binding = 1) uniform RoughnessBlock{
        float roughness;
};

in VS_OUT{
    vec3 fragPos_world;
}fs_in;

layout( binding = 0 ) uniform samplerCube environmentMap;

const float PI = 3.14159265359;


float VanDerCorpusSequence(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 HammersleySequence(uint i, uint N)
{
    return vec2(float(i)/float(N), VanDerCorpusSequence(i));
}

vec3 LobeBiasedSample(vec2 Xi, vec3 N_world, float roughness)
{
    float a = roughness*roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N_world.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N_world));
    vec3 bitangent = cross(N_world, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N_world * H.z;
    return normalize(sampleVec);
}

void main() {
    vec3 N_world = normalize(fs_in.fragPos_world);
    vec3 R_world = N_world;
    vec3 V_world = R_world;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = HammersleySequence(i, SAMPLE_COUNT);
        vec3 H_world  = LobeBiasedSample(Xi, N_world, roughness);
        vec3 L_world  = normalize(2.0 * dot(V_world, H_world) * H_world - V_world);

        float NdotL = max(dot(N_world, L_world), 0.0);
        if(NdotL > 0.0)
        {
            prefilteredColor += texture(environmentMap, L_world).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    fragColor = vec4(prefilteredColor, 1.0);
}
