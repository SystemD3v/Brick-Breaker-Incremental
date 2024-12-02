#version 330 core

// Output fragment color
out vec4 fragColor;

// Uniforms
uniform vec2 iResolution; // Viewport resolution (width, height)
uniform float iTime;      // Time in seconds

#define TOTAL 100.0

// Noise function to generate a 2D pattern
vec2 N22(vec2 p) {
    vec3 a = fract(p.xyx * vec3(863.245, 193.35, 219.731));
    a += dot(a, a + 71.47);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

// Main function to generate the effect
void main() {
    // Normalize coordinates to the [-1, 1] range
    vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / iResolution.xy;

    vec3 col = vec3(0.0);

    float minD = 100.0;
    float cid = 0.0;

    float m = 0.0;
    float t = iTime * 0.3 + 100.0; // Time-based transformation

    // Loop to generate the pattern
    for (float i = 0.0; i < TOTAL; i++) {
        vec2 n = N22(vec2(i * 64.25, i * 19.99)) + cos(uv.x + t) * 0.05;
        vec2 p = cos(n * t) * 0.35;
        float d = length(uv - p);

        m += smoothstep(0.02, 0.01, d);

        if (d < minD) {
            minD = d;
            cid = i;
        }
    }

    // Color base based on time
    vec3 base = sin(t * vec3(5.0, 4.0, 6.0) + 2.0) * 0.4 + 0.8;

    col -= minD * base * 3.0; // Apply the minimum distance-based effect
    col += cid / TOTAL; // Add color based on the ID

    // Output the final color
    fragColor = vec4(col, 1.0);
}
