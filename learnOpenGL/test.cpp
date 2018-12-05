#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
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

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//检测按键
		processInput(window);

		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色
		glClear(GL_COLOR_BUFFER_BIT);


		glfwPollEvents();//检查是否触发事件,并调用对应的回调函数
		glfwSwapBuffers(window);//在迭代中用于绘制
		

	}

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