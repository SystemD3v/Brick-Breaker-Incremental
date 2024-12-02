#version 330 core

// Output fragment color
out vec4 fragColor;

// Uniforms
uniform vec2 iResolution; // Viewport resolution (width, height)
uniform float iTime;      // Time in seconds

// Palette function to generate colors based on distance
vec3 palette(float d) {
    return mix(vec3(0.2, 0.7, 0.9), vec3(1.0, 0.0, 1.0), d);
}

// Rotation function for 2D vectors
vec2 rotate(vec2 p, float a) {
    float c = cos(a);
    float s = sin(a);
    return p * mat2(c, s, -s, c);
}

// Distance function to generate a 3D shape
float map(vec3 p) {
    for (int i = 0; i < 8; ++i) {
        float t = iTime * 0.2;
        p.xz = rotate(p.xz, t);
        p.xy = rotate(p.xy, t * 1.89);
        p.xz = abs(p.xz);
        p.xz -= 0.5;
    }
    return dot(sign(p), p) / 5.0;
}

// Raymarching function to compute color and shading
vec4 rm(vec3 ro, vec3 rd) {
    float t = 0.0;
    vec3 col = vec3(0.0);
    float d;
    for (float i = 0.0; i < 64.0; i++) {
        vec3 p = ro + rd * t;
        d = map(p) * 0.5;
        if (d < 0.02) {
            break;
        }
        if (d > 100.0) {
            break;
        }
        col += palette(length(p) * 0.1) / (400.0 * (d));
        t += d;
    }
    return vec4(col, 1.0 / (d * 100.0));
}

void main() {
    // Normalize fragment coordinates
    vec2 uv = (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.x;

    // Camera position and orientation
    vec3 ro = vec3(0.0, 0.0, -50.0);
    ro.xz = rotate(ro.xz, iTime);

    // Forward, side, and up directions for the camera
    vec3 cf = normalize(-ro);
    vec3 cs = normalize(cross(cf, vec3(0.0, 1.0, 0.0)));
    vec3 cu = normalize(cross(cf, cs));

    // Create ray direction using the camera and fragment coordinates
    vec3 uuv = ro + cf * 3.0 + uv.x * cs + uv.y * cu;
    vec3 rd = normalize(uuv - ro);

    // Perform raymarching and get the final color
    vec4 col = rm(ro, rd);

    // Set the final fragment color
    fragColor = col;
}
