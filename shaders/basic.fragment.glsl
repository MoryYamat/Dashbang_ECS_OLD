#version 330 core

in vec2 vUV;
in vec3 vNormal;
in float vWeaponMask; // ★追加

out vec4 FragColor;

uniform bool uHasBaseColorTex;
uniform sampler2D uBaseColorTex;
uniform vec3 uBaseColor;

uniform float uWeaponVfxStrength; // 0 or 1
uniform float uTime;

float hash12(vec2 p)
{
    float h = dot(p, vec2(127.1, 311.7));
    return fract(sin(h) * 43758.5453123);
}

void main()
{
    vec3 base = uHasBaseColorTex ? texture(uBaseColorTex, vUV).rgb : uBaseColor;

    vec3 n = normalize(vNormal);
    float ndl = clamp(dot(n, normalize(vec3(0,1,0))), 0.2, 1.0);

    // ---- デバッグが通ったら上の if を消してここから下を使う ----
    float s = uWeaponVfxStrength * clamp(vWeaponMask, 0.0, 1.0);

    vec2 uv = vUV * 20.0 + vec2(uTime * 0.7, uTime * 0.4);
    float noise = hash12(floor(uv));

    float pulse = 0.5 + 0.5 * sin(uTime * 8.0);
    float glow = (0.15 + 0.35 * noise) * pulse;

    vec3 vfxColor = vec3(1.2, 1.0, 0.6);

    vec3 lit = base * ndl;
    lit += vfxColor * glow * s;

    FragColor = vec4(lit, 1.0);
}
