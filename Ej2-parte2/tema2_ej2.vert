#version 150 compatibility 
uniform mat4 mvp;
uniform float escala;
uniform bool escala_enable;
uniform float rotate_colors;
uniform float discard_factor;
in vec3 aPosition;
in vec3 aNormal;
in vec2 aTextura;
out vec2 vST;

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


//Matriz de rotacion general
mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    return mat4((1-cos(angle)) * axis.x * axis.x + cos(angle),           (1-cos(angle)) * axis.x * axis.y - axis.z *  sin(angle),  (1-cos(angle)) * axis.z * axis.x + axis.y *  sin(angle),  0.0,
                (1-cos(angle)) * axis.x * axis.y + axis.z *  sin(angle),  (1-cos(angle)) * axis.y * axis.y + cos(angle),           (1-cos(angle)) * axis.y * axis.z - axis.x *  sin(angle),  0.0,
                (1-cos(angle)) * axis.z * axis.x - axis.y *  sin(angle),  (1-cos(angle)) * axis.y * axis.z + axis.x *  sin(angle),  (1-cos(angle)) * axis.z * axis.z + cos(angle),           0.0,
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
    vST = aTextura.st;
}
