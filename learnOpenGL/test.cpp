#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
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

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//��ⰴ��
		processInput(window);

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ
		glClear(GL_COLOR_BUFFER_BIT);


		glfwPollEvents();//����Ƿ񴥷��¼�,�����ö�Ӧ�Ļص�����
		glfwSwapBuffers(window);//�ڵ��������ڻ���
		

	}

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