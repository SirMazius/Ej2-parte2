#version 150 compatibility 
uniform mat4 mvp;
uniform float escala;
uniform bool escala_enable;
uniform float rotate_colors;
in vec3 aPosition;
in vec3 aNormal;
in vec3 aTextura;

out vec3 vColor;

float map() 
{
	//Y = (X-A)/(B-A) * (D-C) + C
	return (escala / 1 * (1-3) + 3);
}

vec3 escale() 
{
	if (!escala_enable)
		return (aPosition);
	else if (distance(aPosition * map(), vec3(0,0,0)) < 3.0)
		return aPosition * map();
	else
		return (normalize(aPosition) * 3);
}



mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 rotate(vec3 v, vec3 axis, float angle) {
	mat4 m = rotationMatrix(axis, angle);
	return (m * vec4(v, 1.0)).xyz;
}

void main()
{
    gl_Position = mvp * vec4(escale(), 1);
    vColor = rotate(aNormal,vec3(1,1,0),rotate_colors);
}
