#version 150 compatibility 
uniform mat4 mvp;
uniform float escala;
uniform bool escala_enable;
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

void main()
{
    gl_Position = mvp * vec4(escale(), 1);
    vColor = aNormal;
}
