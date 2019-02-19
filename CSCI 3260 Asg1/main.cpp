///////////////////////////////////////////
////Type your name and student ID here/////
////Name: LEE Kwun Po
////Student ID: 1155093052

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
GLuint vao[5];
GLuint vbo[5];
GLfloat r, dx, dy, dz, dxevil, dyevil, k, n, m = 0.0f;
GLfloat fov = 62.0f;
GLfloat q = 1.0f;
GLfloat w = 0.1f;
GLfloat e = 10.0f;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;
	glUseProgram(programID);

}

void keyboard(unsigned char key, int x, int y)
{
	//TODO:
	if (key == 'a')
	{
		r += 0.1f;
		dx -= 0.03f;
		dyevil -= 0.03f;

	}
	if (key == 'd')
	{
		r -= 0.1f;
		dx += 0.03f;
		dyevil += 0.03f;
	}
	if (key == 'w')
	{
		r += 0.1f;
		dy += 0.03f;
		dxevil -= 0.03f;

	}
	if (key == 's')
	{
		r -= 0.1f;
		dy -= 0.03f;
		dxevil += 0.03f;
	}
	if (key == 'r')
	{
		r, dx, dy, dxevil, dyevil, k = 0.0f; // reset
	}
	if (key == 'o')
	{
		fov += 1.0f;
	}
	if (key == 'p')
	{
		fov -= 1.0f;
	}
	if (key == 'i')
	{
		q += 1.0f;
	}
	if (key == 'k')
	{
		q -= 1.0f;
	}
	if (key == 'l')
	{
		w += 1.0f;
	}
	if (key == ';')
	{
		w -= 1.0f;
	}
	if (key == '[')
	{
		e += 1.0f;
	}
	if (key == ']')
	{
		e -= 1.0f;
	}
	if (key == 'f')
	{
		x += 0.1f;
	}
	if (dx == -0.2f || dy == 0.6f || dx == 1.0f || dy == -0.6f)
	{
		dz = -0.4f;
	}
	if (r == 1.0f)
	{
		r, dx, dy, dxevil, dyevil, k = 0.0f; // Lost
	}
	if (dx - dxevil == 0.8f) // Winning!!!!
	{
		while (k != 100.0f)
		{
			while (k != 1.0f)
			{
				k += 0.1f;
			}
			while (k != 0.0f)
			{
				k -= 0.1f;
			}
		}
	}
}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs

	//using vao[5] & vbo[5]

	glGenVertexArrays(5, vao);
	glGenBuffers(5, vbo);

	GLuint vbo_idx[3];
	glGenBuffers(3, vbo_idx);

	const GLfloat floor[] =
	{
		-0.5f, -0.5f, -0.5f, //0 bottom left
		+1.0f, +1.0f, +1.0f, // color

		+0.5f, -0.5f, -0.5f, //1 bottom right
		+1.0f, +1.0f, +1.0f, // color
		
		+0.5f, +0.5f, -0.5f, //2 right upper
		+1.0f, +1.0f, +1.0f, // color
		
		-0.5f, +0.5f, -0.5f, //3 left upper
		+1.0f, +1.0f, +1.0f, // color
	
	};

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//indices
	GLushort indicesfloor[] = { 0, 1, 2, 0,3,2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesfloor), indicesfloor, GL_STATIC_DRAW);


	const GLfloat Nonexcitedsupercube[] =
	{
		-0.5f, +0.1f, -0.5f, //0 top left
		+1.0f, +0.0f, +0.0f, // color

		-0.5f, -0.1f, -0.5f, //1 bottom left
		+1.0f, +0.0f, +0.0f, // color

		-0.3f, +0.1f, -0.5f, //2 top right
		+1.0f, +0.0f, +0.0f, // color

		-0.3f, -0.1f, -0.5f, //3 bottom right
		+1.0f, +0.0f, +0.0f, // color
		
		-0.5f, +0.1f, -0.3f, //4 upper top left
		+1.0f, +0.0f, +0.0f, // color

		-0.5f, -0.1f, -0.3f, //5 upper bottom left
		+1.0f, +0.0f, +0.0f, // color

		-0.3f, +0.1f, -0.3f, //6 upper top right
		+1.0f, +0.0f, +0.0f, // color

		-0.3f, -0.1f, -0.3f, //7 upper bottom right
		+1.0f, +0.0f, +0.0f, // color

	};

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Nonexcitedsupercube), Nonexcitedsupercube, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 *sizeof (float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	GLushort indicescube[] = {  0, 1, 2,  1, 3, 2,
								0, 1, 4,  1, 5, 4,
								1, 3, 5,  3, 7, 5,
								2, 3, 6,  3, 7, 6, 
								0, 2, 4,  2, 6, 4, 
								4, 5, 6,  5, 7, 6 
							};
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicescube), indicescube, GL_STATIC_DRAW);


	const GLfloat Blueevilcube[] =
	{
		+0.1f, +0.3f, -0.5f, //0
		+0.0f, +0.0f, +1.0f, //color

		+0.1f, +0.1f, -0.5f, //1
		+0.0f, +0.0f, +1.0f, //color

		+0.3f, +0.3f, -0.5f, //2
		+0.0f, +0.0f, +1.0f, //color

		+0.3f, +0.1f, -0.5f, //3
		+0.0f, +0.0f, +1.0f, //color

		+0.1f, +0.3f, -0.3f, //4
		+0.0f, +0.0f, +1.0f, //color

		+0.1f, +0.1f, -0.3f, //5
		+0.0f, +0.0f, +1.0f, //color

		+0.3f, +0.3f, -0.3f, //6
		+0.0f, +0.0f, +1.0f, //color

		+0.3f, +0.1f, -0.3f, //7
		+0.0f, +0.0f, +1.0f, //color

	};

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Blueevilcube), Blueevilcube, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicescube), indicescube, GL_STATIC_DRAW);


	const GLfloat linewordingX[] =
	{
		+0.5f, +0.7f, +0.0f,//0
		+1.0f, +0.0f, +0.0f,//color

		+0.3f, +0.5f, +0.0f,//1
		+1.0f, +0.0f, +0.0f,//color

		+0.3f, +0.7f, +0.0f,//2
		+1.0f, +0.0f, +0.0f,//color

		+0.5f, +0.5f, +0.0f,//3
		+1.0f, +0.0f, +0.0f,//color

	};
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(linewordingX), linewordingX, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


	const GLfloat pointdot[] =
	{
		+0.2f, +0.5f, +0.0f,//0
		+1.0f, +0.0f, +0.0f,//color	
	};

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointdot), pointdot, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


}


void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint loc1 = glGetUniformLocation(programID, "transformMatrix");
	GLint loc2 = glGetUniformLocation(programID, "projectionMatrix");
	GLint loc3 = glGetUniformLocation(programID, "Colorx");
	GLint loc4 = glGetUniformLocation(programID, "Colory");
	GLint loc5 = glGetUniformLocation(programID, "cancellingx");
	GLint loc6 = glGetUniformLocation(programID, "cancellingy");

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(fov, q, w, e) * glm::lookAt(glm::vec3(1.6f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // using float fov, q, w, e
	glUniformMatrix4fv(loc2, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniform1f(loc3, r);//using float r
	glUniform1f(loc4, k);//using float k

	glm::mat4 translate, rotate, scaling = glm::mat4(1.0f);
	glm::mat4 transformMatrix = translate * rotate * scaling;

	glBindVertexArray(vao[0]);
	m = k;

	n = 1.0f;
	glUniform1f(loc5, n);//using float n
	glUniform1f(loc6, m);//using float m
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(vao[1]);
	n = r;

	m = 1.0f;
	translate = glm::translate(mat4(1.0f), vec3(dx, dy, dz));// using float dx, dy, dz
	transformMatrix = translate;
	glUniformMatrix4fv(loc1, 1, GL_FALSE, &transformMatrix[0][0]);
	glUniform1f(loc5, n);//using float n
	glUniform1f(loc6, m);//using float m
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(vao[2]);
	m = k;

	translate = glm::translate(mat4(1.0f), glm::vec3(dxevil, dyevil, 0.0f));// using float dxevil, dyevil
	transformMatrix = translate;
	glUniformMatrix4fv(loc1, 1, GL_FALSE, &transformMatrix[0][0]);
	glUniform1f(loc6, m);//using float m
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(vao[3]);

	GLfloat x = 0.01f;
	translate = glm::translate(mat4(1.0f), vec3(x, 0.0f, 0.0f));// translate always
	transformMatrix = translate;
	if (x == 0.8f) {
		x = -1.2f;
	}

		rotate = glm::rotate(mat4(1.0f), r, vec3(0.0f, 0.0f, 1.0f));//rotate
		scaling = glm::scale(mat4(1.0f), vec3(1.2f, 1.2f, 0.0f));// scaling
		transformMatrix = translate * rotate * scaling;

	glUniformMatrix4fv(loc1, 1, GL_FALSE, &transformMatrix[0][0]);
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, 8);

	glBindVertexArray(vao[4]);

	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, 2);

	x += 0.01f;

	glFlush();
	glutPostRedisplay();
}


void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	//glutInitWindowPosition()
	glutCreateWindow(argv[0]); // window title
	glewInit();

	initializedGL();

	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	glutMainLoop(); //call display callback over and over
}