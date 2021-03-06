#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb_image.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include"Camera.h"
#include"Mesh.h"
#include"Model.h"

//视口调整回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//按键检测是否按下ESC
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const*path);
//相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

//时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//灯光位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() 
{
	glfwInit();//初始化GLFW
	//配置GLFW，第一个参数代表选项的名称,第二个参数接受一个整形
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号=3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号=3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式
	
	//创建第一个窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "MyFirstOpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//openGL渲染窗口的尺寸
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	Shader ourShader("Shader/model.vs", "Shader/model.fs");
	/*Shader lightingShader("Shader/MultipleLight.vs", "Shader//MultipleLight.fs");
	Shader lampShader("Shader/1.lamp.vs", "Shader/1.lamp.fs");*/
	Model ourModel("Model/nanosuit.obj");
	//float vertices[] = {
	//	//顶点位置-------    -----法线---------    ----纹理---
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  0.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  0.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	//	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f,  0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  1.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  0.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f,  0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	//};
	//unsigned int VBO, cubeVAO;
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindVertexArray(cubeVAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	//unsigned int lightVAO;
	//glGenVertexArrays(1, &lightVAO);
	//glBindVertexArray(lightVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//unsigned int diffuseMap = loadTexture("Texture/container2.png");
	//lightingShader.use();
	//lightingShader.setInt("material.diffuse", 0);//绑定纹理
	//unsigned int specularMap = loadTexture("Texture/container2_specular.png");
	//lightingShader.use();
	//lightingShader.setInt("material.specular", 1);
	
	unsigned int diffuseMap = loadTexture("Texture/body_dif.png");
	ourShader.use();
	ourShader.setInt("material.texture_diffuse1", 0);
	unsigned int specularMap = loadTexture("Texture/body_showroom_spec.png");
	ourShader.use();
	ourShader.setInt("material.texture_specular1", 1);
	unsigned int glassMap = loadTexture("Texture/glass_dif.png");
	ourShader.use();
	ourShader.setInt("material.glass_diffuse", 2);

	
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//记录deltatime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//检测按键
		processInput(window);

		//渲染指令
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//lightingShader.setFloat("material.shininess", 64.0f);

		////定向光
		//lightingShader.setVec3("dirLight.direction", -0.2f, -0.1f, -0.3f);
		//lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		////点光源
		//lightingShader.setVec3("pointLight.position", 0.7f, 0.2f, 2.0f);
		//lightingShader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
		//lightingShader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
		//lightingShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
		//lightingShader.setFloat("pointLight.constant", 1.0f);
		//lightingShader.setFloat("pointLight.linear", 0.09);
		//lightingShader.setFloat("pointLight.quadratic", 0.032);

		////聚光
		//lightingShader.setVec3("spotLight.position", camera.Position);
		//lightingShader.setVec3("spotLight.direction", camera.Front);
		//lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		//lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		//lightingShader.setFloat("spotLight.constant", 1.0f);
		//lightingShader.setFloat("spotLight.linear", 0.09);
		//lightingShader.setFloat("spotLight.quadratic", 0.032);
		//lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);

		//glm::mat4 model;
		//lightingShader.setMat4("model", model);

		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//lampShader.use();
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);
		//model = glm::mat4();
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f));
		//lampShader.setMat4("model", model);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 64.0f);
		//定向光
		ourShader.setVec3("dirLight.direction", -0.2f, -0.1f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		//聚光
		ourShader.setVec3("spotLight.position", camera.Position);
		ourShader.setVec3("spotLight.direction", camera.Front);
		ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("spotLight.constant", 1.0f);
		ourShader.setFloat("spotLight.linear", 0.09);
		ourShader.setFloat("spotLight.quadratic", 0.032);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, glassMap);

		glfwPollEvents();//检查是否触发事件,并调用对应的回调函数
		glfwSwapBuffers(window);//在迭代中用于绘制
		

	}

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/

	glfwTerminate();//释放资源
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
//鼠标回调
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
//滚轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
//读取纹理
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
		//设置环绕方式以及过滤方式
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