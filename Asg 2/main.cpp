/*********************************************************
FILE : main.cpp (csci3260 2018-2019 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155093052
Student Name: LEE Kwun Po
*********************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
// Could define the Vao&Vbo and interaction parameter here
GLuint VertexArrayID, vertexbuffer, uvbuffer, normalbuffer, Texture, TextureID;
GLuint drawSize, drawplane, drawblock;
GLuint VertexArrayID1, VertexArrayID2;
GLuint Texturex, Texture1, Texture2, Texture3, TextureBlock;
float beta = 0.0f;
float aheadx = 0.0f;
float aheadz = 0.0f;
float theta = 0.0f;
float diffuseBrightness = 1.0f;
float specularBrightness = 1.0f;
float spotBrightness = 1.0f;
float ax = 0.0f;
float ay = 0.0f;
float x_old = 0.0f;
float y_old = 0.0f;
float moving = 2.3f;
float a = 3.0f;
float b = 3.0f;
int down = 1;
bool Mouse = false, rotation = false;
//a series utilities for setting shader parameters 
void setMat4(const std::string &name, glm::mat4& value)
{
	unsigned int transformLoc = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void setVec4(const std::string &name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string &name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

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
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
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

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == '1')
		Texturex = Texture1;
	else if (key == '2')
		Texturex = Texture2;
	else if (key == '3')
		Texturex = Texture3;
	else if (key == ' ')
		Mouse = !Mouse;
	else if (key == 'q')
		diffuseBrightness += 0.1f;
	else if (key == 'w')
		diffuseBrightness -= 0.1f;
	else if (key == 'z')
		specularBrightness += 0.1f;
	else if (key == 'x')
		specularBrightness -= 0.1f;
	else if (key == 'c')
		spotBrightness += 0.1f;
	else if (key == 'v')
		spotBrightness -= 0.1f;
	else if (key == 'a')
		a += 0.1f;
	else if (key == 'b')
		b += 0.1f;
	else if (key == 'd')
		a -= 0.1f;
	else if (key == 'f')
		b -= 0.1f;
	else if (key == 's')
		rotation = !rotation;
}

void move(int key, int x, int y) 
{
	//TODO: Use arrow keys to do interactive events and animation
	if (key == GLUT_KEY_UP) {
		aheadx += 0.1f * cos(theta);
		aheadz += 0.1f * sin(theta);
	}
	else if (key == GLUT_KEY_DOWN) {
		aheadx -= 0.1f * cos(theta);
		aheadz -= 0.1f * sin(theta);
	}
	else if (key == GLUT_KEY_LEFT) {
		theta -= 0.1f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		theta += 0.1f;
	}

}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation
	if (Mouse && Mouse)
	{

		if (x_old > x)
			ax -= 0.2f;
		else if (x_old < x)
			ax += 0.2f;
		else
			ax = ax;
		if (y_old > y)
			ay -= 0.2f;
		else if (y_old < y)
			ay += 0.2f;
		else
			ay = ay;
		x_old = x;
		y_old = y;
	}

}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 6 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; 
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}

GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width*height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	//TODO: Create one OpenGL texture and set the texture parameter 

	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
		GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version
	delete[] data;

	glGenerateMipmap(GL_TEXTURE_2D);
	
	return textureID;
}

void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture
	std::vector <glm::vec3> vertices;
	std::vector <glm::vec2> uvs;
	std::vector <glm::vec3> normals;

	bool res = loadOBJ("sources/jeep.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
		&vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	drawSize = vertices.size();
	Texture = loadBMP_custom("sources/jeep_texture.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();

	//plane
	res = loadOBJ("sources/plane.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VertexArrayID1);
	glBindVertexArray(VertexArrayID1);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
		&vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	for (auto& v : uvs) v *= 2;
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	drawplane = vertices.size();
	Texture1 = loadBMP_custom("sources/theme1.bmp");
	Texture2 = loadBMP_custom("sources/theme2.bmp");
	Texture3 = loadBMP_custom("sources/theme3.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texturex);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();

	// block
	res = loadOBJ("sources/block.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VertexArrayID2);
	glBindVertexArray(VertexArrayID2);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
		&vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec2), &normals[0],
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	drawblock = vertices.size();
	TextureBlock = loadBMP_custom("sources/block_texture.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureBlock);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();
}

void paintGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.5f, 0.5f, 0.5f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	
	GLint lightPositionUniformLocation =
		glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPosition(2.0f, 10.0f, -10.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint spotPositionUniformLocation =
		glGetUniformLocation(programID, "spotPositionWorld");
	vec3 spotPosition(a, b, b);
	glUniform3fv(spotPositionUniformLocation, 1, &spotPosition[0]);

	GLint cutoff = glGetUniformLocation(programID, "cutoffangle");
	float cutoffangle = glm::cos(glm::radians(30.0f));
	glUniform1f(cutoff, cutoffangle);

	GLint brightnessofspot = glGetUniformLocation(programID, "spotBrightness");
	glUniform1f(brightnessofspot, spotBrightness);

	GLint brightnessofdiffuse = glGetUniformLocation(programID, "diffuseBrightness");
	glUniform1f(brightnessofdiffuse, diffuseBrightness);

	GLint brightnessofspecular = glGetUniformLocation(programID, "specularBrightness");
	glUniform1f(brightnessofspecular, specularBrightness);

	GLint eyePositionUniformLocation =
		glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition(-8.0f, 0.0f, -10.0f);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	glm::mat4 View = glm::mat4(1.0f);
	View = glm::lookAt(glm::vec3(ax, -ay * 2 +28.0f, 15.0f), glm::vec3(ax, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelScalingMatrix = glm::mat4(1.0f);
	GLint modelScalingMatrixUniformLocation =
		glGetUniformLocation(programID, "modelScalingMatrix");
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	glm::mat4 modelRotationMatrix = glm::mat4(1.0f);
	GLint modelRotationMatrixUniformLocation =
		glGetUniformLocation(programID, "modelRotationMatrix");
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	GLint modelTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(20.0f, 1.0f, 0.5f, 100.0f) * View;
	GLuint projectionMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(VertexArrayID);

	modelTransformMatrix = glm::translate(glm::mat4(),
		glm::vec3(aheadx, 0.0f, aheadz));;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);



	modelRotationMatrix = glm::rotate(glm::mat4(), 
		-glm::radians(theta* 61.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelScalingMatrix = glm::scale(glm::mat4(),
		glm::vec3(0.5f, 0.5f, 0.5f));;
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, Texture);
	glDrawArrays(GL_TRIANGLES, 0, drawSize);

	//plane
	glBindVertexArray(VertexArrayID1);  
	modelTransformMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	modelRotationMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelScalingMatrix = glm::mat4(1.0f);
	modelScalingMatrix = glm::scale(glm::mat4(),
		glm::vec3(3.0f, 0.5f, 3.0f));;
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, Texturex);
	glDrawArrays(GL_TRIANGLES, 0, drawplane);

	//block
	glBindVertexArray(VertexArrayID2);
	modelScalingMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::mat4(1.0f);
	modelRotationMatrix = glm::mat4(1.0f);

	modelScalingMatrix = glm::scale(glm::mat4(),
		glm::vec3(1.5f, 1.5f, 1.5f));;
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	modelRotationMatrix = glm::rotate(glm::mat4(),
		beta, glm::vec3(0, 1, 0))* glm::rotate(glm::mat4(), beta, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelTransformMatrix = glm::translate(glm::mat4(),
		glm::vec3(-5.0f + (15.0f * cos(beta)), moving, -5.0f + (15.0f * sin(beta))));;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	if (rotation)
	beta += 0.001f;

	if (moving < 2.3f && down == 5)
		moving += 0.001f;
	//if (moving > 1.6f && down == 1)
		//moving -= 0.001f;
	if (moving == 1.6f)
		down = 5;
	if (moving == 2.3f)
		down = 1;
	glBindTexture(GL_TEXTURE_2D, TextureBlock);
	glDrawArrays(GL_TRIANGLES, 0, drawblock);




	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	glewInit();
	installShaders();
	sendDataToOpenGL();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Assignment 2");
	glewInit();
	
	//TODO:
	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutPassiveMotionFunc(PassiveMouse);
	
	glutMainLoop();

	return 0;
}