#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb_image.h>
#include"Shader.h"

//�ӿڵ����ص�����
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
//��������Ƿ���ESC
void processInput(GLFWwindow *window);



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
	
	Shader ourShader("texture.vs", "texture.fs");


	//float vertices[] = {
	//	//---����λ��---   ---��ɫ����---
	//	-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
	//	 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,
	//	 0.0f, 0.5f,0.0f,0.0f,0.0f,1.0f
	//};

	////�������㻺����󣬶����������
	//unsigned int VBO, VAO;
	//glGenVertexArrays(1,&VAO);
	//glGenBuffers(1, &VBO);
	////�Ȱ�VAO,Ȼ��󶨲�����VBO��Ȼ�����ö�������
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////���ö�������
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	////���
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//-----------������������
	float vertices[] = {
		//---λ������    ---��ɫ����---    ---��������---
		 0.5f, 0.5f,0.0f, 1.0f,0.0f,0.0f,  1.0f,1.0f,
		 0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,  1.0f,0.0f,
		-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,  0.0f,0.0f,
		-0.5f, 0.5f,0.0f, 1.0f,1.0f,0.0f,  0.0f,1.0f,
	};
	unsigned int indices[] = {
		0,1,3,//��һ��������
		1,2,3 //�ڶ���������
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);//����EBO��

	//----��ȡ����
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
	glBindTexture(GL_TEXTURE_2D, texture);
	//Ϊ��ǰ�󶨵�����������û��ƣ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//���ز���������
	int width, height, nrChannels;
	unsigned char*data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//��ⰴ��
		processInput(window);

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);

		//��������
		ourShader.use();
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);//����������������������
		glBindVertexArray(VAO);//��VAO;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//������������������������

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
}