#pragma once

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "ShaderProgram.h"
#include "Camera.h"
using namespace std;

class HW7 {

public:
	HW7(const unsigned int width, const unsigned int height, ShaderProgram lshader,
		ShaderProgram depthShader, ShaderProgram shadowShader);
	~HW7();
	void imguiSetting();
	void renderHW7(Camera camera);
	void setSize(const unsigned int width, const unsigned int height);
	void initialize();

private:
	unsigned int VBO, cubeVAO; //立方体
	unsigned int lightVAO;    // 光源

	ShaderProgram lightShader;
	ShaderProgram depthShader;
	ShaderProgram shadowShader;

	int width, height;
	int e = 0;

	bool change = false;
	bool start_camera = false;
	bool is_run = false;

	glm::vec3 lightPos;
	float lpos[3] = { -5.0f, 5.0f, -5.0f };
	float fov = 45.0f;

	float ambientStrength = 0.1;
	float diffuseStrength = 1.0;
	float specularStrength = 0.5;
	int shininess = 32;

	const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	GLuint depthMapFBO;
	GLuint depthMap;
	float plane[2] = { 1.0f, 30.0f };
	GLfloat near_plane = 1.0f, far_plane = 30.0f;

	float vertices[216] = {   //顶点数组
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,

		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  0.0f,  1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f,  1.0f,

		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,

		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f

	};

};
