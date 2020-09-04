#version 120

#ifdef GL_ES
precision mediump float;
#endif

uniform bool useTex;
uniform sampler2D texture;
uniform vec4 color;

varying float alpha;

void main()
{
    if (alpha <= 0.0)
    {
        discard;
    }
    else
    {
        vec4 tmpColor = vec4(color.rgb, alpha);
        if(!useTex) {
            gl_FragColor = tmpColor;
        } else {
            gl_FragColor = texture2D(texture, gl_PointCoord) * tmpColor;
        }
    }
}