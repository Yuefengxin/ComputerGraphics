#pragma once

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "ShaderProgram.h"
#include "Camera.h"
using namespace std;

class HW6 {

public:
	HW6(const unsigned int width, const unsigned int height, ShaderProgram lshader,
		ShaderProgram pshader, ShaderProgram gshader);
	void imguiSetting();
	void renderHW6(Camera camera);
	void setSize(const unsigned int width, const unsigned int height);

private:
	ShaderProgram lightShader;
	ShaderProgram phongShader;
	ShaderProgram gouraudShader;

	int width, height;
	int e = 0;

	bool change = false;
	bool start_camera = true;
	bool is_run = false;

	glm::vec3 lightPos;
	float lpos[3] = { 0.0f, 0.0f, 4.0f };
	float fov = 45.0f;

	float ambientStrength = 0.1;
	float diffuseStrength = 1.0;
	float specularStrength = 0.5;
	int shininess = 32;

	float vertices[216] = {   //¶¥µãÊý×é
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
