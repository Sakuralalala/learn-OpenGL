#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb_image.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include"Camera.h"


//�ӿڵ����ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//��������Ƿ���ESC
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const*path);
//���
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

//ʱ��
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//�ƹ�λ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() 
{
	glfwInit();//��ʼ��GLFW
	//����GLFW����һ����������ѡ�������,�ڶ�����������һ������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��=3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��=3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�ú���ģʽ
	
	//������һ������
	GLFWwindow* window = glfwCreateWindow(800, 600, "MyFirstOpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//��GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//openGL��Ⱦ���ڵĳߴ�
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	//Shader ourShader("Shader/texture.vs", "Shader/texture.fs");
	Shader lightingShader("Shader/Spotlight.vs", "Shader/Spotlight.fs");
	Shader lampShader("Shader/1.lamp.vs", "Shader/1.lamp.fs");

	float vertices[] = {
		//����λ��-------    -----����---------    ----����---
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("Texture/container2.png");
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);//������
	unsigned int specularMap = loadTexture("Texture/container2_specular.png");
	lightingShader.use();
	lightingShader.setInt("material.specular", 1);


	
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//��¼deltatime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//��ⰴ��
		processInput(window);

		//��Ⱦָ��
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("light.position", camera.Position);
		lightingShader.setVec3("light.direction", camera.Front);
		lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

		lightingShader.setVec3("viewPos", camera.Position);
		//���ղ�������
		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.09f);
		lightingShader.setFloat("light.quadratic", 0.032f);
		//���ʲ�������
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 64.0f);



		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glm::mat4 model;
		lightingShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		//����ת��
		//glm::mat4 model;
		//glm::mat4 view;
		//glm::mat4 projection;
		////��ǰƽ����x����ת-55��
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f, 0.3f));
		////�۲���󣬼�������ǰ�ƶ�3.0f(-z��)
		////view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
		//view = camera.GetViewMatrix();
		////͸�Ӿ���
		//projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		//
		//ourShader.setMat4("model", model);
		//ourShader.setMat4("view", view);
		//ourShader.setMat4("projection", projection);


		//��������
		//ourShader.use();
		/*unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));*/
		
		/*glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);*///����������������������
		//glBindVertexArray(VAO);//��VAO;
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//������������������������

		glfwPollEvents();//����Ƿ񴥷��¼�,�����ö�Ӧ�Ļص�����
		glfwSwapBuffers(window);//�ڵ��������ڻ���
		

	}

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	glfwTerminate();//�ͷ���Դ
	return 0;

}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
//���ص�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//���ֻص�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
//��ȡ����
unsigned int loadTexture(char const*path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) 
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//���û��Ʒ�ʽ�Լ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}