/*********************************************************
FILE : main.cpp (csci3260 2018-2019 Project)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155093052
Student Name: LEE Kwun Po
*********************************************************/

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

GLint programID, Skybox_programID;
// Could define the Vao&Vbo and interaction parameter here
GLuint skyboxVAO, skyboxVBO, sky_cubemapTexture;
GLuint VertexArrayID, vertexbuffer, uvbuffer, normalbuffer, Texture, TextureID;
GLuint drawSize, drawplane, drawblock, texture_normal;
GLuint VertexArrayID1, VertexArrayID2, V[3];
GLuint drawring[3], Texturering[3], i;
GLuint Texturex, Texture1, Texture2, Texture3,Texture4, TextureBlock,texture_normal1;
glm::mat4 Model_matrix = glm::mat4(1.0f);
glm::mat4 SC_Rot_M = glm::mat4(1.0f);
float SCInitialPos[3] = { 10.0f, 0.0f, 100.0f };
float SCTranslation[3] = { 1.0f, 0.0f, 1.0f };
glm::vec4 SC_world_pos, SC_world_Front_Direction, SC_world_Right_Direction, Camera_world_position;
glm::vec3 SC_local_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 SC_local_right = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 SC_local_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera_local_position = glm::vec3(0.0f, 0.3f, 0.1f);
GLuint drawrock[200], Texturerock[200], Vrocks[200];

glm::mat4 *modelMatrices;
int amount = 200;

float displacement1[200];
float beta = 0.0f;
float aheadx = 0.0f;
float aheadz = 0.0f;
float theta = 0.0f;
float theta1 = 0.0f;
float rotationtheta = 0.0f;
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
bool normalMapping;
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

	
	//skybox_programID
	GLuint vertexShaderID1 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID1 = glCreateShader(GL_FRAGMENT_SHADER);

	temp = readShaderCode("skybox_VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID1, 1, adapter, 0);
	temp = readShaderCode("skybox_FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID1, 1, adapter, 0);

	glCompileShader(vertexShaderID1);
	glCompileShader(fragmentShaderID1);

	if (!checkShaderStatus(vertexShaderID1) || !checkShaderStatus(fragmentShaderID1))
		return;

	Skybox_programID = glCreateProgram();
	glAttachShader(Skybox_programID, vertexShaderID);
	glAttachShader(Skybox_programID, fragmentShaderID);
	glLinkProgram(Skybox_programID);

	if (!checkProgramStatus(Skybox_programID))
		return;

	glDeleteShader(vertexShaderID1);
	glDeleteShader(fragmentShaderID1);
	

}

void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
    if (key == 'q')
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
	if (key == GLUT_KEY_UP)
	{
		SCTranslation[0] = SCTranslation[0] - 0.5 * SC_world_Front_Direction[0];
		SCTranslation[2] = SCTranslation[2] - 0.5 * SC_world_Front_Direction[2];
		printf("%.f, ", SCTranslation[0]);
		printf("%.f\n", SCTranslation[2]);
		if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -40.0f & SCTranslation[2] <= -39.0f))
		{
			TextureBlock = Texture2;
			Texturering[2] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -60.0f & SCTranslation[2] <= -59.0f))
		{
			TextureBlock = Texture2;
			Texturering[1] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -80.0f &  SCTranslation[2] <= -79.0f))
		{
			TextureBlock = Texture2;
			Texturering[0] = Texture2;
		}
		//collision Detection
		//earth
		else if ((SCTranslation[0] >= -12.0f & SCTranslation[0] <= -7.0f) & (SCTranslation[2] >= -100.0f &  SCTranslation[2] <= -95.0f))
		{
			TextureBlock = Texturex;
		}
		//rocks and wonderplanet
		else if ((SCTranslation[0] >= -20.0f & SCTranslation[0] <= 0.0f) & (SCTranslation[2] >= -20.0f &  SCTranslation[2] <= 20.0f))
		{
			TextureBlock = Texturex;
		}
		//detection end
		else
		{
			TextureBlock = Texture3;
			Texturering[0] = Texture4;
			Texturering[1] = Texture4;
			Texturering[2] = Texture4;
		}

	}
	else if (key == GLUT_KEY_DOWN)
	{
		SCTranslation[0] = SCTranslation[0] + 0.5 * SC_world_Front_Direction[0];
		SCTranslation[2] = SCTranslation[2] + 0.5 * SC_world_Front_Direction[2];
		if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -40.0f & SCTranslation[2] <= -39.0f))
		{
			TextureBlock = Texture2;
			Texturering[2] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -60.0f & SCTranslation[2] <= -59.0f))
		{
			TextureBlock = Texture2;
			Texturering[1] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -80.0f &  SCTranslation[2] <= -79.0f))
		{
			TextureBlock = Texture2;
			Texturering[0] = Texture2;
		}
		//collision Detection
		//earth
		else if ((SCTranslation[0] >= -12.0f & SCTranslation[0] <= -7.0f) & (SCTranslation[2] >= -100.0f &  SCTranslation[2] <= -95.0f))
		{
			TextureBlock = Texturex;
		}
		//rocks and wonderplanet
		else if ((SCTranslation[0] >= -20.0f & SCTranslation[0] <= 0.0f) & (SCTranslation[2] >= -20.0f &  SCTranslation[2] <= 20.0f))
		{
			TextureBlock = Texturex;
		}
		//detection end
		else
		{
			TextureBlock = Texture3;
			Texturering[0] = Texture4;
			Texturering[1] = Texture4;
			Texturering[2] = Texture4;
		}
	}
	else if (key == GLUT_KEY_LEFT) 
	{
		SCTranslation[0] = SCTranslation[0] + 0.5 * SC_world_Right_Direction[0];
		if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -40.0f & SCTranslation[2] <= -39.0f))
		{
			TextureBlock = Texture2;
			Texturering[2] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -60.0f & SCTranslation[2] <= -59.0f))
		{
			TextureBlock = Texture2;
			Texturering[1] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -80.0f &  SCTranslation[2] <= -79.0f))
		{
			TextureBlock = Texture2;
			Texturering[0] = Texture2;
		}
		//collision Detection
		//earth
		else if ((SCTranslation[0] >= -12.0f & SCTranslation[0] <= -7.0f) & (SCTranslation[2] >= -100.0f &  SCTranslation[2] <= -95.0f))
		{
			TextureBlock = Texturex;
		}
		//rocks and wonderplanet
		else if ((SCTranslation[0] >= -20.0f & SCTranslation[0] <= 0.0f) & (SCTranslation[2] >= -20.0f &  SCTranslation[2] <= 20.0f))
		{
			TextureBlock = Texturex;
		}
		//detection end
		else
		{
			TextureBlock = Texture3;
			Texturering[0] = Texture4;
			Texturering[1] = Texture4;
			Texturering[2] = Texture4;
		}
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		SCTranslation[0] = SCTranslation[0] - 0.5 * SC_world_Right_Direction[0];
		if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -40.0f & SCTranslation[2] <= -39.0f))
		{
			TextureBlock = Texture2;
			Texturering[2] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -60.0f & SCTranslation[2] <= -59.0f))
		{
			TextureBlock = Texture2;
			Texturering[1] = Texture2;
		}
		else if ((SCTranslation[0] >= -10.0f & SCTranslation[0] <= -9.0f) & (SCTranslation[2] >= -80.0f &  SCTranslation[2] <= -79.0f))
		{
			TextureBlock = Texture2;
			Texturering[0] = Texture2;
		}
		//collision Detection
		//earth
		else if ((SCTranslation[0] >= -12.0f & SCTranslation[0] <= -7.0f) & (SCTranslation[2] >= -100.0f &  SCTranslation[2] <= -95.0f))
		{
			TextureBlock = Texturex;
		}
		//rocks and wonderplanet
		else if ((SCTranslation[0] >= -20.0f & SCTranslation[0] <= 0.0f) &  (SCTranslation[2] >= -20.0f &  SCTranslation[2] <= 20.0f))
		{
			TextureBlock = Texturex;
		}
		//detection end
		else
		{
			TextureBlock = Texture3;
			Texturering[0] = Texture4;
			Texturering[1] = Texture4;
			Texturering[2] = Texture4;
		}
	}

}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation

		if (x_old > x)
		{
			theta += 1.0f;
			SC_Rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (x_old < x)
		{
			theta -= 1.0f;
			SC_Rot_M = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		x_old = x;
		y_old = y;
	

}


void UpdateStatus() {
	float scale = 0.01;

	glm::mat4 SC_scale_M = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
	glm::mat4 SC_trans_M = glm::translate
	(
		glm::mat4(1.0f),
		glm::vec3(SCInitialPos[0] + SCTranslation[0], SCInitialPos[1] + SCTranslation[1], SCInitialPos[2] + SCTranslation[2])
	);
	Model_matrix = SC_trans_M*SC_Rot_M*SC_scale_M;
	SC_world_pos = Model_matrix * glm::vec4(SC_local_front, 1.0f);
	SC_world_Front_Direction = Model_matrix * glm::vec4(SC_local_front, 1.0f);
	SC_world_Right_Direction = Model_matrix * glm::vec4(SC_local_right, 1.0f);
	SC_world_Front_Direction = normalize(SC_world_Front_Direction);
	SC_world_Right_Direction = normalize(SC_world_Right_Direction);
	Camera_world_position = Model_matrix * glm::vec4(Camera_local_position, 1.0f);
}

void CreateRand_ModelM()
{
	modelMatrices = new glm::mat4[amount];
	// initialize random seed
	srand(glutGet(GLUT_ELAPSED_TIME));
	GLfloat radius = 6.0f;
	GLfloat offset = 2.5f;
	GLfloat displacement;
	for (i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. Translation: Randomly displae along circle with radius 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		// x
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		// y
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f;
		// z
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi) randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		//4. Now add to list of matrices
		modelMatrices[i] = model;
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

/*unsigned char* loadBMP_data(const char* imagepath1, unsigned char* image1, int width1, int height1) 
{
	printf("Reading skybox face %s\n", imagepath1);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;

	FILE * file = fopen(imagepath1, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath1); getchar(); return 0; }

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
	width1 = *(int*)&(header[0x12]);
	height1 = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width1*height1 * 3;
	if (dataPos == 0)      dataPos = 54;

	image1 = new unsigned char[imageSize];
	fread(image1, 1, imageSize, file);
	fclose(file);

	return width1, height1, image1;
}*/


GLuint loadCubemap(vector<const GLchar*> faces)
{
	int width, height;
	unsigned char* image;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	GLuint textureID11;
	glGenTextures(1, &textureID11);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID11);
	for (i = 0; i < faces.size(); i++)
	{
		printf("Reading skybox face %s\n", faces[i]);

		FILE * file = fopen(faces[i], "rb");
		if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", faces[i]); getchar(); return 0; }

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

		image = new unsigned char[imageSize];
		fread(image, 1, imageSize, file);
		fclose(file);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 
					 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID11;
}

void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture
	
	//Cubemap
	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//loadCubemap
	vector<const GLchar*> sky_faces;
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_rt.bmp");
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_lf.bmp");
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_up.bmp");
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_dn.bmp");
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_bk.bmp");
	sky_faces.push_back("object models and textures/texture/universe skybox/purplenebula_ft.bmp");

	sky_cubemapTexture = loadCubemap(sky_faces);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_cubemapTexture);
	glUniform1i(glGetUniformLocation(Skybox_programID, "skybox"), 0);
	
	//objectssss
	std::vector <glm::vec3> vertices;
	std::vector <glm::vec2> uvs;
	std::vector <glm::vec3> normals;


	//earth
	bool res = loadOBJ("object models and textures/planet.obj", vertices, uvs, normals);
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
	Texture = loadBMP_custom("object models and textures/texture/earthTexture.bmp");
	texture_normal = loadBMP_custom("object models and textures/texture/earth_normal.bmp");

	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	texture_normal1 = glGetUniformLocation(programID, "myTextureSampler_normal");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glUniform1i(texture_normal1, 1);

	vertices.clear(); uvs.clear(); normals.clear();

	//wonderplanet
	res = loadOBJ("object models and textures/planet.obj", vertices, uvs, normals);
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
	Texture1 = loadBMP_custom("object models and textures/texture/WonderStarTexture.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture1);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();

	// spacecraft
	res = loadOBJ("object models and textures/spaceCraft.obj", vertices, uvs, normals);
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
	TextureBlock = loadBMP_custom("object models and textures/texture/spacecraftTexture.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureBlock);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();

	// ring
	for(i =0; i <=2; i++)
	{
	res = loadOBJ("object models and textures/Ring.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &V[i]);
	glBindVertexArray(V[i]);

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
	drawring[i] = vertices.size();
	Texturering[i] = loadBMP_custom("object models and textures/texture/ringTexture.bmp");
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texturering[i]);
	glUniform1i(TextureID, 0);

	vertices.clear(); uvs.clear(); normals.clear();
	}

	// rocks
	for (i = 0; i <= 199; i++)
	{
		res = loadOBJ("object models and textures/rock.obj", vertices, uvs, normals);
		glGenVertexArrays(1, &Vrocks[i]);
		glBindVertexArray(Vrocks[i]);

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
		drawrock[i] = vertices.size();
		Texturerock[i] = loadBMP_custom("object models and textures/texture/RockTexture.bmp");
		TextureID = glGetUniformLocation(programID, "myTextureSampler");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texturerock[i]);
		glUniform1i(TextureID, 0);

		vertices.clear(); uvs.clear(); normals.clear();
	}


	//visual feedback
	Texture2 = loadBMP_custom("object models and textures/texture/RockTexture.bmp");
	Texture3 = TextureBlock;
	Texture4 = Texturering[2];

	for (i = 0;i <= 199;i++)
	{
		displacement1[i] = (rand() % (int)(2 * 2.5f * 100)) / 100.0f - 2.5f;
	}
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
	
	UpdateStatus();
	//adding another light sources
	GLint lightPositionUniformLocation1 =
		glGetUniformLocation(programID, "lightPositionWorld1");
	vec3 lightPosition1(0.0f, 0.0f, 0.0f);
	glUniform3fv(lightPositionUniformLocation1, 1, &lightPosition1[0]);


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
	//vec3 eyePosition = Camera_local_position;
	vec3 eyePosition(-8.0f, 0.0f, 10.0f);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	glm::mat4 View = glm::mat4(1.0f);
	View = glm::lookAt(glm::vec3(Camera_world_position), glm::vec3(SC_world_pos), glm::vec3(0.0f, 1.0f, 0.0f));
	//View = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

	glm::mat4 modelRotationMatrix1 = glm::mat4(1.0f);
	GLint modelRotationMatrixUniformLocation1 =
		glGetUniformLocation(programID, "modelRotationMatrix1");
		glUniformMatrix4fv(modelRotationMatrixUniformLocation1, 1,
			GL_FALSE, &modelRotationMatrix1[0][0]);

	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	GLint modelTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(60.0f, 1.0f, 1.0f, 100.0f) * View;
	GLuint projectionMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	
	//draw skybox
	glUseProgram(Skybox_programID);
	glDisable(GL_DEPTH_TEST);

	GLuint Skb_ModelUniformLocation = glGetUniformLocation(Skybox_programID, "M");
	glm::mat4 Skb_ModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(Skb_ModelUniformLocation, 1, GL_FALSE, &Skb_ModelMatrix[0][0]);
	//Remove any translation component of the view matrix
	glm::mat4 view1 = glm::mat4(glm::mat3(View));
	glm::mat4 projection = glm::perspective(60.0f, 1.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Skybox_programID, "view"), 1, GL_FALSE, glm::value_ptr(view1));
	glUniformMatrix4fv(glGetUniformLocation(Skybox_programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// skybox cube
	glBindVertexArray(skyboxVAO);

	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
	
	
	//objectsss
	glUseProgram(programID);
	//earth
	glBindVertexArray(VertexArrayID);

	modelTransformMatrix = glm::translate(glm::mat4(),
		glm::vec3(0.0f, 0.0f, 0.0f));;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);


	modelRotationMatrix = glm::rotate(glm::mat4(),
		-glm::radians(rotationtheta* 61.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelScalingMatrix = glm::scale(glm::mat4(),
		glm::vec3(1.5f, 1.5f, 1.5f));;
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	normalMapping = true;
	GLint normalMapping_position = glGetUniformLocation(programID, "normalMapping_flag");
	glUniform1i(normalMapping_position, normalMapping);

	glBindTexture(GL_TEXTURE_2D, Texture);
	//glBindTexture(GL_TEXTURE_2D, texture_normal);
	glDrawArrays(GL_TRIANGLES, 0, drawSize);

	normalMapping = false;
	//wonderplanet
	glBindVertexArray(VertexArrayID1);  
	modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::translate(glm::mat4(),
		glm::vec3(0.0f, 0.0f, 100.0f));;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	modelRotationMatrix = glm::rotate(glm::mat4(),
		-glm::radians(rotationtheta* 61.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelScalingMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, Texture1);
	glDrawArrays(GL_TRIANGLES, 0, drawplane);

	//spacecraft
	glBindVertexArray(VertexArrayID2);
	modelScalingMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::mat4(1.0f);
	modelRotationMatrix = glm::mat4(1.0f);


	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
		GL_FALSE, &modelScalingMatrix[0][0]);

	glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
		GL_FALSE, &modelRotationMatrix[0][0]);

	modelTransformMatrix = Model_matrix;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);



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

	//ring
	for (i = 0; i <= 2; i++)
	{
		glBindVertexArray(V[i]);
		modelScalingMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::mat4(1.0f);
		modelRotationMatrix = glm::mat4(1.0f);

		modelScalingMatrix = glm::scale(glm::mat4(),
			glm::vec3(0.05f, 0.05f, 0.05f));;
		glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
			GL_FALSE, &modelScalingMatrix[0][0]);



		modelRotationMatrix1 = glm::rotate(glm::mat4(),
			glm::radians(90.0f), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelRotationMatrixUniformLocation1, 1,
			GL_FALSE, &modelRotationMatrix1[0][0]);

		modelRotationMatrix = glm::rotate(glm::mat4(),
			-glm::radians(rotationtheta* 61.0f), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
			GL_FALSE, &modelRotationMatrix[0][0]);



		modelTransformMatrix = glm::translate(glm::mat4(),
			glm::vec3(0.0f, 0.0f, ((20 * i) + 20.0f)));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
			GL_FALSE, &modelTransformMatrix[0][0]);

		glBindTexture(GL_TEXTURE_2D, Texturering[i]);
		glDrawArrays(GL_TRIANGLES, 0, drawring[i]);
	}

	//rocks
	for (i = 0; i <= 199; i++)
	{
		glBindVertexArray(Vrocks[i]);
		modelScalingMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::mat4(1.0f);
		modelRotationMatrix = glm::mat4(1.0f);

		modelScalingMatrix = glm::scale(glm::mat4(),
			glm::vec3(0.1f, 0.1f, 0.1f));;
		glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1,
			GL_FALSE, &modelScalingMatrix[0][0]);

		modelRotationMatrix = glm::rotate(glm::mat4(),
			-glm::radians(rotationtheta* 61.0f), glm::vec3(0, 0, 1));
		glUniformMatrix4fv(modelRotationMatrixUniformLocation, 1,
			GL_FALSE, &modelRotationMatrix[0][0]);

		modelTransformMatrix = glm::translate(glm::mat4(),
			glm::vec3((10.0f * cos(beta)) + displacement1[i], moving, displacement1[i] + 100.0f + (10.0f * sin(beta))));;
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
			GL_FALSE, &modelTransformMatrix[0][0]);

		glBindTexture(GL_TEXTURE_2D, Texturerock[i]);
		glDrawArrays(GL_TRIANGLES, 0, drawrock[i]);


		beta += 0.05f;
		
	}
	beta = 0.0f;
	rotationtheta += 0.001f;
	
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
	glutCreateWindow("Project");
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