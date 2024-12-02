#version 330 core

uniform vec2 iResolution;
uniform float iTime;

out vec4 FragColor;

void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = fragCoord / iResolution.xy;

    FragColor = vec4(uv, 0.5 + 0.5 * sin(iTime), 1.0);
}
