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

	//������
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(cubeVAO);
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��֮ǰ����Ķ������ݸ��Ƶ�������ڴ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//ʹ��glVertexAttribPointer��������OpenGL����ν�����������
	//λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ��Դ
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ����ӵ�VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���õ�������Ķ������ԣ��Թ�Դ��˵����ֻ��λ�����ݣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Ϊ��Ⱦ�������ͼ����һ��֡�������
	glGenFramebuffers(1, &depthMapFBO);
	//����һ��2D�����ṩ��֡�������Ȼ���ʹ��
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//�Ľ���������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//�����ɵ����������Ϊ֡�������Ȼ���
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//�������κ���ɫ���ݽ�����Ⱦ
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HW7::renderHW7(Camera camera) {
	// ����imgui
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

	// 1. ��ѡ��Ⱦ�����ͼ
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
	glCullFace(GL_FRONT);  //�޸�peter����
	//Render Scene
	//����������
	model = glm::mat4(1.0f);
	depthShader.setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//����ƽ��
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // ����ƽ��
	model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f)); // ����
	depthShader.setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glCullFace(GL_BACK);   //�޸�peter����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. ������һ����Ⱦ�����������ʹ�������ͼ
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (start_camera) {
		// ʹ�����������͸�Ӿ���
		fov = camera.zoom;
		projection = glm::perspective(glm::radians(camera.zoom),
			(float)width / (float)height, 0.1f, 100.0f);
		// ������ӽǱ仯
		view = camera.GetViewMatrix();

	}
	else {
		projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -40.0f));
	}
	view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, -1.0f, 0.0f));
	model = glm::mat4(1.0f);
	//����������
	shadowShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix); // ���ձ任
	shadowShader.setInt("shadowMap", 0); // ��Ӱ���ͼ,������ɫ�����ڵ�����Ԫ

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
	//���Ƶذ�
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); // ����ƽ��
	model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f)); // ����
	shadowShader.setMat4("model", model);
	shadowShader.setVec3("objectColor", 0.8f, 0.9f, 0.8f); // ������ɫ

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//���ƹ�Դ
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
