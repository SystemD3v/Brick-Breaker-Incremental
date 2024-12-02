#version 330 core

// Output fragment color
out vec4 fragColor;

// Uniforms
uniform vec2 iResolution; // Viewport resolution (width, height)
uniform float iTime;      // Time in seconds

// Function to generate color palette based on the input value
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    // Normalize fragment coordinates
    vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv; // Save original UVs for later use

    vec3 finalColor = vec3(0.0);

    // Loop for generating multiple layers (like the original shader)
    for (float i = 0.0; i < 4.0; i++) {
        // Distort texture coordinates
        uv = fract(uv * 1.5) - 0.5;

        // Compute distance to the origin
        float d = length(uv) * exp(-length(uv0));

        // Get color from the palette function based on distance and time
        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);

        // Modify distance with a sine function for animation
        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);

        // Adjust the intensity of the effect
        d = pow(0.01 / d, 1.2);

        // Accumulate color based on modified distance
        finalColor += col * d;
    }

    // Set the final color, with a gamma correction (sqrt)
    fragColor = vec4(sqrt(finalColor), 1.0);
}
