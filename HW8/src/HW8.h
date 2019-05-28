#pragma once

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <vector>
#include "ShaderProgram.h"
#include "Camera.h"
using namespace std;

class HW8 {

public:
	HW8(const unsigned int width, const unsigned int height, ShaderProgram myshader, GLFWwindow* window);
	~HW8();
	void imguiSetting();
	void renderHW8();
	void setSize(const unsigned int width, const unsigned int height);
	void initialize();

private:

	ShaderProgram myShader;
	GLFWwindow* window;

	int width, height;
	int e = 0;
	int computedFac = 1;
	long long int factorialTmp[105];

	bool animate = false;
	bool status = false;
	bool is_run = false;
	bool is_color = false;
	bool leftMouse = false;
	bool rightMouse = false;

	float vertices[2500];
	float frameT;
	vector<glm::vec2> pointsIn;
	vector<glm::vec2> pointsOut;

	ImVec4 color_setting = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	void generateVertices(vector<glm::vec2> v);
	long long int calFactorial(int i);
	float Bernstein(int i, int n, float t);
	void drawPoints(int count, int size = 10, bool subline = false);
};
