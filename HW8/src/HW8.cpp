#include "HW8.h"

double posX, posY;
bool leftButton, rightButton;

HW8::HW8(const unsigned int height, const unsigned int width, ShaderProgram myshader, GLFWwindow* _window)
	:myShader(myshader) {

	this->height = height;
	this->width = width;
	window = _window;

	initialize();
}

//����ƶ��ص�����
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	posX = xpos;
	posY = ypos;
}

//������ص�����
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	leftButton = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT;
	rightButton = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT;
}

HW8::~HW8() {

}

void HW8::setSize(const unsigned int width, const unsigned int height) {
	this->width = width;
	this->height = height;
}

void HW8::imguiSetting() {
	if (ImGui::BeginMenu("HW8"))
	{
		ImGui::MenuItem("Bezier Curve", NULL, &is_run);
		ImGui::MenuItem("Color Setting", NULL, &is_color);

		ImGui::EndMenu();
	}
	
}

void HW8::initialize() {
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	
	factorialTmp[0] = 1;

}

void HW8::renderHW8() {
	// ����imgui
	if (is_color) {
		ImGui::Begin("Color Setting", &is_color);
		ImGui::ColorEdit3("Choose color", (float*)&color_setting, 1);
		ImGui::End();
	}
	if (is_run)
	{
		ImGui::Begin("Homework8", &is_run);
		
		ImGui::Text("Choose the projection type");
		ImGui::Checkbox("Animating", &animate);

		ImGui::End();
	}
	else {
		leftMouse = false;
		rightMouse = false;
		return;
	}

	glViewport(0, 0, width, height);
	// �������
	if (leftMouse == false && leftButton == true) {
		if (pointsIn.size() < 21) {
			pointsIn.push_back(glm::vec2(posX, posY));
		}
	}
	leftMouse = leftButton;
	// �Ҽ�����
	if (rightMouse == false && rightButton == true || animate != status) {
		if (pointsIn.size() > 0) 
			pointsIn.pop_back();
	}
	status = animate;
	rightMouse = rightButton;
	// ���ƿ��Ƶ�
	generateVertices(pointsIn);
	int num = pointsIn.size();
	myShader.use();
	myShader.setVec3("ourColor", color_setting.x, color_setting.y, color_setting.z);
	drawPoints(num);
	myShader.setVec3("ourColor", 1.0, 1.0, 1.0);
	drawPoints(num, 5, true);

	// ��̬����
	if (num > 1 && animate) {
		frameT += 0.001;
		if (frameT > 1) frameT = 0;
		float t = frameT;

		myShader.setVec3("ourColor", 0.86, 0.08, 0.24);
		glm::vec2 p = pointsIn[0] * Bernstein(0, pointsIn.size() - 1, t);
		for (int i = 1; i < this->pointsIn.size(); i++) {
			p = p + pointsIn[i] * Bernstein(i, pointsIn.size() - 1, t);
		}
		pointsOut.push_back(p);
		generateVertices(pointsOut);
		drawPoints(pointsOut.size());
		pointsOut.clear();

		myShader.setVec3("ourColor", 1.0, 1.0, 1.0);
		pointsOut.insert(pointsOut.end(), pointsIn.begin(), pointsIn.end());
		while (num > 1) {
			int newCount = 0;
			for (int i = 0; i < num - 1; i++) {
				glm::vec2 p = pointsOut[i] * (1 - t) + pointsOut[i + 1] * t;
				pointsOut.push_back(p);
			}
			pointsOut.erase(pointsOut.begin(), pointsOut.begin() + num);
			generateVertices(pointsOut);
			drawPoints(pointsOut.size(), 5, true);
			
			num--;
		}
		pointsOut.clear();
	}

	//����beizer����
	num = pointsIn.size();
	myShader.setVec3("ourColor", color_setting.x, color_setting.y, color_setting.z);
	if (num > 1) {
		for (float t = 0; t < 1; t += 0.001) {
			glm::vec2 p = pointsIn[0] * Bernstein(0, num - 1, t);
			for (int i = 1; i < num; i++) {
				p = p + pointsIn[i] * Bernstein(i, num - 1, t);
			}
			pointsOut.push_back(p);
		}
		generateVertices(pointsOut);
		drawPoints(pointsOut.size(), 6);
		pointsOut.clear();
	}
}

// ���ɶ�������
void HW8::generateVertices(vector<glm::vec2> points) {
	for (int i = 0; i < points.size(); i++) {
		vertices[i * 2 + 0] = (points[i].x / width)*2 - 1;
		vertices[i * 2 + 1] = -((points[i].y / height)*2 - 1);
	}
}

//����׳�
long long int HW8::calFactorial(int i) {
	if (i > 104) return 1;
	for (; computedFac <= i; computedFac++) {
		factorialTmp[computedFac] = factorialTmp[computedFac - 1] * computedFac;
	}
	return factorialTmp[i];
}

// ���� Bernstein ������
float HW8::Bernstein(int i, int n, float t) {
	long long int a = calFactorial(i) * calFactorial(n - i);
	float res = calFactorial(n) / a;
	res *= pow(t, i) * pow(1 - t, n - i);
	return res;
}

void HW8::drawPoints(int count, int size, bool subline) {
	if (count < 1) return;
	unsigned int VBO, VAO;
	//����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ��Ĵ�С
	glPointSize(size);
	//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//ʹ��glVertexAttribPointer��������OpenGL����ν�����������
	//λ��
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// �����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_POINTS, 0, count);
	if (subline) {
		glDrawArrays(GL_LINE_STRIP, 0, count);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
}