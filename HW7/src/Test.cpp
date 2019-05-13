#include "HW3.h"
#include "HW4.h"
#include "HW5.h"
#include "HW6.h"
#include "HW7.h"

using namespace std;

#define WIDTH 1280
#define HEIGHT 800

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 40.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
//�ص���������ÿ�δ��ڴ�С��������ʱ�򱻵���
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

ImVec4 color_setting = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

int main()
{
	//��ʼ��GLFW
	glfwInit();
	//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//���汾��(Major)�ʹΰ汾��(Minor)����Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ʹ�ú���ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//����һ�����ڶ������ÿ�ߡ�����
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//ע��ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//ShaderProgram ourShader;
	//ourShader.use();
	//����ImGui����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	ShaderProgram shader1("ShaderPro/shader.vs", "ShaderPro/shader.fs");
	ShaderProgram shader2("ShaderPro/camera.vs", "ShaderPro/camera.fs");
	ShaderProgram lightShader("ShaderPro/light.vs", "ShaderPro/light.fs");
	//ShaderProgram shader4("ShaderPro/light.vs", "ShaderPro/light.fs");
	ShaderProgram phongShader("ShaderPro/phong.vs", "ShaderPro/phong.fs");
	//ShaderProgram shader5("ShaderPro/light.vs", "ShaderPro/light.fs");
	ShaderProgram gouraudShader("ShaderPro/gouraud.vs", "ShaderPro/gouraud.fs");
	ShaderProgram simpleDepthShader("ShaderPro/simpleDepthShader.vs", "ShaderPro/simpleDepthShader.fs");
	ShaderProgram shadowShader("ShaderPro/shadowShader.vs", "ShaderPro/shadowShader.fs");

	HW3 homework3(WIDTH, HEIGHT, shader1);
	HW4 homework4(WIDTH, HEIGHT, shader2);
	HW5 homework5(WIDTH, HEIGHT, shader2);
	HW6 homework6(WIDTH, HEIGHT, lightShader, phongShader, gouraudShader);
	HW7 homework7(WIDTH, HEIGHT, lightShader, simpleDepthShader, shadowShader);

	//��Ⱦѭ��(Render Loop)
	while (!glfwWindowShouldClose(window)) //ѭ����ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//�������
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����ɫ�������Ȼ���

		ImGui_ImplGlfwGL3_NewFrame();
		
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Selection"))
			{
				homework3.imguiSetting();
				homework4.imguiSetting();
				homework5.imguiSetting();
				homework6.imguiSetting();
				homework7.imguiSetting();
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}
		
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		homework3.setSize(display_w, display_h);
		homework4.setSize(display_w, display_h);
		homework5.setSize(display_w, display_h);
		homework6.setSize(display_w, display_h);
		homework7.setSize(display_w, display_h);

		homework3.renderHW3();
		homework4.renderHW4();
		homework5.renderHW5(camera);
		homework6.renderHW6(camera);
		homework7.renderHW7(camera);

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		//�����û�д���ʲô�¼�
		glfwSwapBuffers(window);
		//������ɫ����
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//�������
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// hw5
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// ����ƶ��ص�����
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = WIDTH / 2.0f;
		lastY = HEIGHT / 2.0f;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// �������ص�����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}