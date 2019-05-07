#include "HW6.h"

HW6::HW6(const unsigned int height, const unsigned int width, ShaderProgram lshader, 
	ShaderProgram pshader, ShaderProgram gshader)
	:lightShader(lshader), phongShader(pshader), gouraudShader(gshader) {

	this->height = height;
	this->width = width;

}

void HW6::setSize(const unsigned int width, const unsigned int height) {
	this->width = width;
	this->height = height;
}

void HW6::imguiSetting() {
	ImGui::MenuItem("HW6", NULL, &is_run);
}

void HW6::renderHW6(Camera camera) {
	// ����imgui
	if (is_run)
	{
		ImGui::Begin("Homework6", &is_run);

		ImGui::Text("Choose the shading type");
		ImGui::RadioButton("Phong Shading", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("Gouraud Shading", &e, 1);

		ImGui::Text("Adjust parameters");
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 2.0f);
		ImGui::SliderFloat("Diffuse Strength", &diffuseStrength, 0.0f, 2.0f);
		ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 2.0f); 
		ImGui::SliderInt("Shininess", &shininess, 0, 256);
		
		ImGui::SliderFloat3("Light Position", lpos, -15.0f, 15.0f);
		ImGui::Checkbox("Light keeps changing", &change);
		ImGui::Checkbox("Start camera", &start_camera);
		ImGui::SliderFloat("Field of View", &fov, 0.0f, 45.0f);
		ImGui::End();
	}
	else {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	ShaderProgram ourShader;
	if (e == 0) {
		ourShader = phongShader;
	}
	else {
		ourShader = gouraudShader;
	}
	//������
	unsigned int VBO, cubeVAO;
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
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ����ӵ�VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���õ�������Ķ������ԣ��Թ�Դ��˵����ֻ��λ�����ݣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	if (start_camera) {
		// ʹ�����������͸�Ӿ���
		fov = camera.zoom;
		projection = glm::perspective(glm::radians(camera.zoom),
			(float)width / (float)height, 0.1f, 100.0f);
		// ������ӽǱ仯
		view = camera.GetViewMatrix();

	}
	else {
		projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 1.0f, 100.0f);
		// ������ӽǱ仯
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
	}
	view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, -1.0f, 0.0f));
	if (change) {
		float raduis = 10.0f;
		float x = sin((float)glfwGetTime()) * raduis;
		float z = cos((float)glfwGetTime()) * raduis;

		lightPos = glm::vec3(x, lpos[1], z);
	}
	else {
		lightPos = glm::vec3(lpos[0], lpos[1], lpos[2]);
	}

	//����������
	ourShader.use();
	
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("model", model);
	ourShader.setMat4("view", view);

	ourShader.setFloat("ambientStrength", ambientStrength);
	ourShader.setFloat("diffuseStrength", diffuseStrength);
	ourShader.setFloat("specularStrength", specularStrength);
	ourShader.setInt("shininess", shininess);
	
	ourShader.setVec3("lightPos", lightPos);
	ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ourShader.setVec3("objectColor", 1.0f, 0.4f, 0.3f);
	ourShader.setVec3("viewPos", camera.GetPostion());
	
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

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}
