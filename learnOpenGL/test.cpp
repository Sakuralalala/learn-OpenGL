#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<stb_image.h>
#include"Shader.h"

//视口调整回调函数
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
//按键检测是否按下ESC
void processInput(GLFWwindow *window);



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
	
	Shader ourShader("texture.vs", "texture.fs");


	//float vertices[] = {
	//	//---顶点位置---   ---颜色数据---
	//	-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
	//	 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,
	//	 0.0f, 0.5f,0.0f,0.0f,0.0f,1.0f
	//};

	////创建顶点缓冲对象，顶点数组对象
	//unsigned int VBO, VAO;
	//glGenVertexArrays(1,&VAO);
	//glGenBuffers(1, &VBO);
	////先绑定VAO,然后绑定并设置VBO，然后设置顶点属性
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////设置顶点属性
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	////解绑
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//-----------画两个三角形
	float vertices[] = {
		//---位置数据    ---颜色数据---    ---纹理数据---
		 0.5f, 0.5f,0.0f, 1.0f,0.0f,0.0f,  1.0f,1.0f,
		 0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,  1.0f,0.0f,
		-0.5f,-0.5f,0.0f, 0.0f,0.0f,1.0f,  0.0f,0.0f,
		-0.5f, 0.5f,0.0f, 1.0f,1.0f,0.0f,  0.0f,1.0f,
	};
	unsigned int indices[] = {
		0,1,3,//第一个三角形
		1,2,3 //第二个三角形
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

	//glBindVertexArray(0);//保持EBO绑定

	//----读取纹理
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, texture);
	//为当前绑定的纹理对象设置环绕，过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载并生成纹理
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


	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//检测按键
		processInput(window);

		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);

		//画三角形
		ourShader.use();
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);//第三个参数代表画几个顶点
		glBindVertexArray(VAO);//绑定VAO;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//第三个参数代表索引的类型

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
}