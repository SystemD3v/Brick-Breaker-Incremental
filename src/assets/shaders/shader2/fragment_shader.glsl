#version 330 core

out vec4 fragColor; // Output fragment color

uniform vec2 iResolution; // Viewport resolution (width, height)
uniform float iTime;       // Time in seconds

struct Grid {
    vec3 id;
    float d;
};
Grid gr;

#define FBI floatBitsToInt
#define FFBI(a) (FBI(cos(a)) ^ FBI(a))

float hash(vec3 uv) {
    int x = FFBI(uv.x);
    int y = FFBI(uv.y);
    int z = FFBI(uv.z);
    return float((x * x + y) * (y * y - x) * (z * z + x)) / 2.14e9;
}

void dogrid(vec3 ro, vec3 rd, float size) {
    gr.id = (floor(ro + rd * 1E-3) / size + 0.5) * size;
    vec3 src = -(ro - gr.id) / rd;
    vec3 dst = abs(0.5 * size) / rd;
    vec3 bz = src + dst;
    gr.d = min(bz.x, min(bz.y, bz.z));
}

vec3 erot(vec3 p, vec3 ax, float t) {
    return mix(dot(ax, p) * ax, p, cos(t)) + cross(ax, p) * sin(t);
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;
    vec3 col = vec3(0.0);
    vec3 ro = vec3(0.2, 0.2, -5.0), rt = vec3(0.0);
    vec3 z = normalize(rt - ro);
    vec3 x = normalize(cross(z, vec3(0.0, -1.0, 0.0)));
    vec3 y = cross(z, x);
    vec3 rd = mat3(x, y, z) * normalize(vec3(uv, 2.0 + tanh(hash(uv.xyy + iTime) * 0.5 + 10.0 * sin(iTime))));

    float i, e, g;
    float gridlen = 0.0;
    for (i = 0.0, e = 0.01, g = 0.0; i++ < 99.0;) {
        vec3 p = ro + rd * g;
        vec3 oop = p;
        p = erot(p, normalize(sin(iTime * 0.33 + vec3(-0.6, 0.4, 0.2))), iTime * 0.2);
        p.z += iTime;

        vec3 op = p;
        if (gridlen <= g) {
            dogrid(p, rd, 1.0);
            gridlen += gr.d;
        }
        p -= gr.id;
        float gy = dot(sin(gr.id * 2.0), cos(gr.id.zxy * 5.0));
        float rn = hash(gr.id);
        p.x += sin(rn) * 0.25;

        float h = rn > 0.0 ? 0.5 : length(p) - 0.01 - gy * 0.05 + rn * 0.02;

        g += e = max(0.001 + op.z * 0.000002, abs(h));
        col += vec3(0.25, 0.25, 1.0 + abs(rn)) * (0.025 + (0.02 * exp(5.0 * fract(gy + iTime)))) / exp(e * e * i);
    }

    col *= exp(-0.08 * g);
    fragColor = vec4(sqrt(col), 1.0);
}
