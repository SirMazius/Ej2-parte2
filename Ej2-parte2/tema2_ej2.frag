#version 150 compatibility

uniform float discard_factor;
uniform sampler2D stone_texture;
uniform sampler2D moss_texture;
uniform bool discard_factor_bool;
uniform float texture_count;
in vec4 vColor;
in vec2 vST;
out vec4 fFragColor;

void main()
{
    float sf = vST.s * 10.0f;
    float st = vST.t * 10.0f;
    
    // float sf = vST.s * 10.0;
    // float st = vST.t * 10.0;

    if (discard_factor_bool && fract(sf) > discard_factor && fract(st) > discard_factor)
        discard;
    //fFragColor = vColor;
    vec4 stone = texture(stone_texture, vST);
    vec4 moss = texture(moss_texture, vST) * 0.5; // se multiplica para que no sea musgo radioactivo
    if (texture_count == 0)
        fFragColor = stone + vColor;
    else if(texture_count == 1)
        fFragColor = vec4(mix(stone.rgb,moss.rgb,moss.a),1.0);
    else
        fFragColor = vColor;
}