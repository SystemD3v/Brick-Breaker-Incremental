#version 330 core

// Output fragment color
out vec4 fragColor;

// Uniforms
uniform vec2 iResolution; // Viewport resolution (width, height)
uniform float iTime;      // Time in seconds

// Rotation matrix for a given angle
mat2 rot(float a) {
    float c = cos(a), s = sin(a);
    return mat2(c,s,-s,c);
}

// Constants for pi and 2*pi
const float pi = acos(-1.0);
const float pi2 = pi * 2.0;

// Modify coordinates based on modulo and rotation
vec2 pmod(vec2 p, float r) {
    float a = atan(p.x, p.y) + pi / r;
    float n = pi2 / r;
    a = floor(a / n) * n;
    return p * rot(-a);
}

// Box function for distance calculation
float box(vec3 p, vec3 b) {
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

// Recursive function to generate a fractal box shape
float ifsBox(vec3 p) {
    for (int i = 0; i < 5; i++) {
        p = abs(p) - 1.0;
        p.xy *= rot(iTime * 0.3);
        p.xz *= rot(iTime * 0.1);
    }
    p.xz *= rot(iTime);
    return box(p, vec3(0.4, 0.8, 0.3));
}

// Main map function to modify coordinates and apply fractals
float map(vec3 p, vec3 cPos) {
    vec3 p1 = p;
    p1.x = mod(p1.x - 5.0, 10.0) - 5.0;
    p1.y = mod(p1.y - 5.0, 10.0) - 5.0;
    p1.z = mod(p1.z, 16.0) - 8.0;
    p1.xy = pmod(p1.xy, 5.0);
    return ifsBox(p1);
}

// The main function that generates the fractal scene
void main() {
    // Normalize fragment coordinates
    vec2 p = (gl_FragCoord.xy * 2.0 - iResolution.xy) / min(iResolution.x, iResolution.y);

    // Camera position and direction
    vec3 cPos = vec3(0.0, 0.0, -3.0 * iTime);
    vec3 cDir = normalize(vec3(0.0, 0.0, -1.0));
    vec3 cUp  = vec3(sin(iTime), 1.0, 0.0);
    vec3 cSide = cross(cDir, cUp);

    // Calculate the ray direction based on screen coordinates
    vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir);

    // Phantom mode (ray marching)
    float acc = 0.0;
    float acc2 = 0.0;
    float t = 0.0;
    for (int i = 0; i < 99; i++) {
        vec3 pos = cPos + ray * t;
        float dist = map(pos, cPos);
        dist = max(abs(dist), 0.02);
        float a = exp(-dist * 3.0);

        // Phantom mode effect
        if (mod(length(pos) + 24.0 * iTime, 30.0) < 3.0) {
            a *= 2.0;
            acc2 += a;
        }

        acc += a;
        t += dist * 0.5;
    }

    // Calculate final color
    vec3 col = vec3(acc * 0.01, acc * 0.011 + acc2 * 0.002, acc * 0.012 + acc2 * 0.005);
    fragColor = vec4(col, 1.0 - t * 0.03); // Output the color with transparency based on the ray distance
}
