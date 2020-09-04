#version 120

uniform bool useObjRef;
uniform vec2 objRef;
uniform vec2 viewportSize;
uniform vec2 viewportOrigin;
uniform vec2 direction;
uniform bool useDirection;

attribute vec2 position;
attribute vec2 texPos;

varying vec2 texCoord;

void main() {
    vec2 pos = position;
    if(useDirection) {
        float x = pos.x * direction.x - pos.y * direction.y;
        float y = pos.x * direction.y + pos.y * direction.x;
        pos.x = x;
        pos.y = y;
    }

    if(useObjRef) {
        pos += objRef;
    }
    pos -= viewportOrigin;
    gl_Position = vec4(pos * 2.0 / viewportSize, 0.0, 1.0);

    texCoord = texPos;
}
