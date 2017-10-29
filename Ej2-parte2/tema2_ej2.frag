#version 150 compatibility

uniform float discard_factor;
in vec4 vColor;
in vec2 vST;
out vec4 fFragColor;

void main()
{
    float sf = vST.s * 10.0f;
    float st = vST.t * 10.0f;
    
    // float sf = vST.s * 10.0;
    // float st = vST.t * 10.0;

    if (fract(sf) > discard_factor && fract(st) > discard_factor)
        discard;
    fFragColor = vColor;
}