#version 120

uniform vec2 viewportSize;
uniform vec2 viewportOrigin;
uniform vec2 ref;
uniform float curTime;
uniform float initSpeed;
uniform float acceleration;
uniform float particleSize;

attribute vec2 direction;

varying float alpha;

void main()
{
    vec2 pos = ref + direction * (initSpeed * curTime - acceleration * curTime * curTime / 2.0);
    pos -= viewportOrigin;
    gl_Position = vec4(pos * 2.0 / viewportSize, 0.0, 1.0);
    
    alpha = 1.0 - acceleration * curTime / initSpeed;
    if (alpha < 0.0)
    {
        alpha = 0.0;
    }

    gl_PointSize = particleSize;
}