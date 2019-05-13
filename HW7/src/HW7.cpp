#include "HW7.h"

HW7::HW7(const unsigned int height, const unsigned int width, ShaderProgram lshader,
	ShaderProgram _depthShader, ShaderProgram _shadowShader)
	:lightShader(lshader), depthShader(_depthShader), shadowShader(_shadowShader) {

	this->height = height;
	this->width = width;

	initialize();
}

HW7::~HW7() {
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}

void HW7::setSize(const unsigned int width, const unsigned int height) {
	this->width = width;
	this->height = height;
}

void HW7::imguiSetting() {
	ImGui::MenuItem("HW7", NULL, &is_run);
}

void HW7::initialize() {
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//立方体
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(cubeVAO);
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把之前定义的顶点数据复制到缓冲的内存
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
	//位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法向量
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 光源
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//只需要绑定VBO不用再次设置VBO的数据，箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对光源来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//为渲染的深度贴图创建一个帧缓冲对象
	glGenFramebuffers(1, &depthMapFBO);
	//创建一个2D纹理，提供给帧缓冲的深度缓冲使用
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//改进采样过多
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//把生成的深度纹理作为帧缓冲的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//不适用任何颜色数据进行渲染
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HW7::renderHW7(Camera camera) {
	// 设置imgui
	if (is_run)
	{
		ImGui::Begin("Homework7", &is_run);

		ImGui::Text("Choose the projection type");
		ImGui::RadioButton("Orthographic", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("Projection", &e, 1);

		ImGui::SliderFloat2("Near&Far Plane", plane, 0.0f, 100.0f);
		ImGui::SliderFloat3("Light Position", lpos, -15.0f, 15.0f);
		ImGui::Checkbox("Light keeps changing", &change);
		ImGui::Checkbox("Start camera", &start_camera);
		ImGui::SliderFloat("Field of View", &fov, 0.0f, 45.0f);
		ImGui::End();
	}
	else {
		return;
	}

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	if (change) {
		float raduis = 10.0f;
		float x = sin((float)glfwGetTime()) * raduis;
		float z = cos((float)glfwGetTime()) * raduis;

		lightPos = glm::vec3(x, lpos[1], z);
	}
	else {
		lightPos = glm::vec3(lpos[0], lpos[1], lpos[2]);
	}

	// 1. 首选渲染深度贴图
	// Configure Shader And Matrices
	near_plane = plane[0]; far_plane = plane[1];
	glm::mat4 lightProjection;
	if(e == 0){
		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
	}
	else {
		lightProjection = glm::perspective((float)glm::radians(45.0f), 1.0f, near_plane, far_plane);
	}
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(1.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);  //修复peter游移
	//Render Scene
	//绘制立方体
	model = glm::mat4(1.0f);
	depthShader.setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//绘制平面
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // 向下平移
	model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f)); // 缩放
	depthShader.setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glCullFace(GL_BACK);   //修复peter游移
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. 像往常一样渲染场景，但这次使用深度贴图
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (start_camera) {
		// 使用摄像机生成透视矩阵
		fov = camera.zoom;
		projection = glm::perspective(glm::radians(camera.zoom),
			(float)width / (float)height, 0.1f, 100.0f);
		// 摄像机视角变化
		view = camera.GetViewMatrix();

	}
	else {
		projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -40.0f));
	}
	view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, -1.0f, 0.0f));
	model = glm::mat4(1.0f);
	//绘制立方体
	shadowShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix); // 光照变换
	shadowShader.setInt("shadowMap", 0); // 阴影深度图,设置着色器属于的纹理单元

	shadowShader.setMat4("projection", projection);
	shadowShader.setMat4("model", model);
	shadowShader.setMat4("view", view);

	shadowShader.setFloat("ambientStrength", ambientStrength);
	shadowShader.setFloat("diffuseStrength", diffuseStrength);
	shadowShader.setFloat("specularStrength", specularStrength);
	shadowShader.setInt("shininess", shininess);

	shadowShader.setVec3("lightPos", lightPos);
	shadowShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shadowShader.setVec3("objectColor", 1.0f, 0.4f, 0.3f);
	shadowShader.setVec3("viewPos", camera.GetPostion());

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//绘制地板
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // 向下平移
	model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f)); // 缩放
	shadowShader.setMat4("model", model);
	shadowShader.setVec3("objectColor", 0.8f, 0.9f, 0.8f); // 物体颜色

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//绘制光源
	lightShader.use();

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.1f));

	lightShader.setMat4("model", model);
	lightShader.setMat4("view", view);
	lightShader.setMat4("projection", projection);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
