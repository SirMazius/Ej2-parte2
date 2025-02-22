// Ej2-parte2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void initCube();
int initTeapot(int grid, glm::mat4 transform);
int initPlane(float xsize, float zsize, int xdivs, int zdivs);
void drawCube();
void drawTeapot();
void drawPlane();

void loadSource(GLuint &shaderID, std::string name);
void printCompileInfoLog(GLuint shadID);
void printLinkInfoLog(GLuint programID);
void validateProgram(GLuint programID);

bool init();
void display();
void resize(int, int);
void idle();
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void mouse(int, int, int, int);
void mouseMotion(int, int);


bool fullscreen = false;
bool mouseDown = false;
bool animation = false;
bool escala = false;
bool rotate_colors = false;
bool discard = false;
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
float escale_factor;
float rotate_color = 0.0f;
float discard_factor = 0.0f;
int g_Width = 500;                          // Ancho inicial de la ventana
int g_Height = 500;                         // Altura incial de la ventana
int texture_count = 0;
GLuint teapotVAOHandle;
GLuint locUniformMVPM;
GLuint locUniformEscala;
GLuint locUniformMColor;
GLuint locUniformUmbral;
GLuint locUniformUmbralBool;
GLuint locUniformStone, locUniformMoss, locUniformUseTexture, locUniformTexture_count;
GLuint vertexShaderObject, fragmentShaderObject;
GLuint program;
GLuint locUniformEscalabool;
GLuint locUniformRotateColors;
int numVertTeapot;


// BEGIN: Carga shaders ////////////////////////////////////////////////////////////////////////////////////////////

void loadSource(GLuint &shaderID, std::string name)
{
	std::ifstream f(name.c_str());
	if (!f.is_open())
	{
		std::cerr << "File not found " << name.c_str() << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	// now read in the data
	std::string *source;
	source = new std::string(std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>());
	f.close();

	// add a null to the string
	*source += "\0";
	const GLchar * data = source->c_str();
	glShaderSource(shaderID, 1, &data, NULL);
	delete source;
}

void printCompileInfoLog(GLuint shadID)
{
	GLint compiled;
	glGetShaderiv(shadID, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetShaderiv(shadID, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetShaderInfoLog(shadID, infoLength, &chsWritten, infoLog);

		std::cerr << "Shader compiling failed:" << infoLog << std::endl;
		system("pause");
		delete[] infoLog;

		exit(EXIT_FAILURE);
	}
}

void printLinkInfoLog(GLuint programID)
{
	GLint linked;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		GLchar *infoLog = new GLchar[infoLength];
		GLint chsWritten = 0;
		glGetProgramInfoLog(programID, infoLength, &chsWritten, infoLog);

		std::cerr << "Shader linking failed:" << infoLog << std::endl;
		system("pause");
		delete[] infoLog;

		exit(EXIT_FAILURE);
	}
}

void validateProgram(GLuint programID)
{
	GLint status;
	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 0)
		{
			GLchar *infoLog = new GLchar[infoLength];
			GLint chsWritten = 0;
			glGetProgramInfoLog(programID, infoLength, &chsWritten, infoLog);
			std::cerr << "Program validating failed:" << infoLog << std::endl;
			system("pause");
			delete[] infoLog;

			exit(EXIT_FAILURE);
		}
	}
}

// END:   Carga shaders ////////////////////////////////////////////////////////////////////////////////////////////


// BEGIN: Inicializa primitivas de dibujo //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Init Teapot
// parametros: 
//		grid - número de rejillas
//		transform - matriz de tranformación del modelo
// return:
//		número de vertices del modelo
///////////////////////////////////////////////////////////////////////////////
int initTeapot(int grid, glm::mat4 transform)
{
	int verts = 32 * (grid + 1) * (grid + 1);
	int faces = grid * grid * 32;
	float * v = new float[verts * 3];
	float * n = new float[verts * 3];
	float * tc = new float[verts * 2];
	unsigned int * el = new unsigned int[faces * 6];

	generatePatches(v, n, tc, el, grid);
	moveLid(grid, v, transform);

	// Tarea por hacer (ejer. 4): Crear y activar VAO
	glGenVertexArrays(1, &teapotVAOHandle);
	glBindVertexArray(teapotVAOHandle);


	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(float), v, GL_STATIC_DRAW); // Datos de la posición de los vértices
	GLuint loc = glGetAttribLocation(program, "aPosition");																			   // Tarea por hacer (ejer. 4): Establecer el puntero al atributo del vertex shader y activarlo
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, (3 * verts) * sizeof(float), n, GL_STATIC_DRAW); // Datos de las normales de los vértices
	GLuint loc2 = glGetAttribLocation(program, "aNormal");																			   // Tarea por hacer (ejer. 4): Establecer el puntero al atributo del vertex shader y activarlo
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, (2 * verts) * sizeof(float), tc, GL_STATIC_DRAW); // Datos de las coordenadas de textura
	GLuint loc3 = glGetAttribLocation(program, "aTextura");
	glEnableVertexAttribArray(loc3);// Tarea por hacer (ejer. 4): Establecer el puntero al atributo del vertex shader y activarlo
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof(unsigned int), el, GL_STATIC_DRAW); // Array de índices


	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tc;

	// Tarea por hacer (ejer. 4): Desactivar VAO
	glBindVertexArray(0);

	return 6 * faces;
}
// END: Inicializa primitivas ////////////////////////////////////////////////////////////////////////////////////

// BEGIN: Funciones de dibujo ////////////////////////////////////////////////////////////////////////////////////
void drawTeapot() {
	// Tarea por hacer (ejer. 4): Dibujar la tetera
	glBindVertexArray(teapotVAOHandle);
	glDrawElements(GL_TRIANGLES, numVertTeapot, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
// END: Funciones de dibujo ////////////////////////////////////////////////////////////////////////////////////


// BEGIN: GLUT /////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(g_Width, g_Height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Programa Ejemplo");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		system("pause");
		exit(-1);
	}
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();

	return EXIT_SUCCESS;
}

bool init()
{

	glClearColor(0.93f, 0.93f, 0.93f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);


	// Tarea por hacer (ejer. 4): Crear el objeto programa, compilar los shaders, adjuntarlos y linkar
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	loadSource(vertexShaderObject, "tema2_ej2.vert");
	loadSource(fragmentShaderObject, "tema2_ej2.frag");

	glCompileShader(vertexShaderObject);
	printf("vertexShaderObject \n");
	printCompileInfoLog(vertexShaderObject);

	glCompileShader(fragmentShaderObject);
	printf("fragmentShaderObject \n");
	printCompileInfoLog(fragmentShaderObject);

	program = glCreateProgram();

	glAttachShader(program, fragmentShaderObject);
	glAttachShader(program, vertexShaderObject);

	glLinkProgram(program);
	printLinkInfoLog(program);

	validateProgram(program);

	numVertTeapot = initTeapot(10, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f)));


	// Inicializa Texturas
	GLuint textIds[2];
	glGenTextures(2, textIds);

	TGAFILE tgaImage;
	// textura piedra
	if (LoadTGAFile("stone.tga", &tgaImage))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textIds[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tgaImage.imageWidth, tgaImage.imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaImage.imageData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		locUniformStone = glGetUniformLocation(program, "stone_texture");
		// Tarea a realizar (ejercicio 5): Activar la unidad de textura, enlazar el objeto textura, cargar en él los datos de la imagen y definir los parámetros de la textura
	}
	else std::cout << "Error al cargar la textura 'stone.tga'" << std::endl;

	// textura musgo
	if (LoadTGAFile("moss.tga", &tgaImage))
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textIds[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tgaImage.imageWidth, tgaImage.imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tgaImage.imageData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		locUniformMoss = glGetUniformLocation(program, "moss_texture");
		// Tarea a realizar (ejercicio 5): Activar la unidad de textura, enlazar el objeto textura, cargar en él los datos de la imagen y definir los parámetros de la textura
	}
	else std::cout << "Error al cargar la textura 'moss.tga'" << std::endl;

	// Tarea a realizar (ejercicios 4 y 5): localizar las variables uniform definidas en los shaders.

	locUniformMVPM = glGetUniformLocation(program, "mvp");
	locUniformEscala = glGetUniformLocation(program, "escala");
	locUniformEscalabool = glGetUniformLocation(program, "escala_enable");
	locUniformRotateColors = glGetUniformLocation(program, "rotate_colors");
	locUniformUmbral = glGetUniformLocation(program, "discard_factor");
	locUniformUmbralBool = glGetUniformLocation(program, "discard_factor_bool");
	locUniformTexture_count = glGetUniformLocation(program, "texture_count");
	return true;
}

void display()
{
	static float time = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Projection = glm::perspective(45.0f, 1.0f * g_Width / g_Height, 1.0f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	Model = glm::rotate(Model, xrot, glm::vec3(1.0f, 0.0f, 0.0f));
	Model = glm::rotate(Model, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = Projection * View * Model;

	// Tarea por hacer (ejer. 4): Activar el objeto programa
	// Tarea por hacer (ejer. 4): Pasarle al vertex shader mvp y el resto de variables uniform
	glUseProgram(program);

	glUniformMatrix4fv(locUniformMVPM, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1f(locUniformEscala, abs(sin(escale_factor)));
	glUniform1f(locUniformEscalabool, escala);
	glUniform1f(locUniformRotateColors, rotate_color);
	glUniform1f(locUniformUmbral, abs(sin(discard_factor)));
	glUniform1f(locUniformUmbralBool, discard);
	glUniform1f(locUniformTexture_count, texture_count);

	if (locUniformStone >= 0)
		glUniform1i(locUniformStone, 0);

	if (locUniformMoss >= 0)
		glUniform1i(locUniformMoss, 1);

	drawTeapot();
	// Tarea por hacer (ejer. 4): Desactivar el objeto programa	
	glUseProgram(0);

	glutSwapBuffers();
}

void transform_teapot() {

}

void resize(int w, int h)
{
	g_Width = w;
	g_Height = h;
	glViewport(0, 0, g_Width, g_Height);
}

void idle()
{
	if (!mouseDown && animation)
	{
		xrot += 0.3f;
		yrot += 0.4f;
	}

	if (!mouseDown && escala)
	{
		escale_factor += 0.01f;
	}
	else {
		escale_factor = 3.1415926 / 2;
	}

	if (!mouseDown && rotate_colors)
	{
		rotate_color += 0.017f;
	}

	if (discard)
	{
		discard_factor += 0.01;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: case 'q': case 'Q':
		glDeleteProgram(program);
		exit(1);
		break;
	case 'a': case 'A':
		animation = !animation;
		break;
	case 'm': case 'M':
		if (escala)
			escala = false;
		else
			escala = true;
		break;
	case 'c': case 'C':
		if (rotate_colors)
			rotate_colors = false;
		else
			rotate_colors = true;
		break;
	case 'd': case 'D':
		if (discard)
			discard = false;
		else
			discard = true;
		break;
	case 't': case 'T':
		switch (texture_count)
		{
		case 0:
			texture_count++;
			break;
		case 1:
			texture_count++;
			break;
		case 2:
			texture_count = 0;
			break;
		}
		// Tarea por hacer (ejer. 4): Activar las tareas asociadas a las teclas 'v', 'c', 'd' y 't'.
	}
}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(g_Width, g_Height);
			glutPositionWindow(50, 50);
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}




