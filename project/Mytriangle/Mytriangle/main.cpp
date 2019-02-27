#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);//渲染窗口的大小
void processInput(GLFWwindow *window);//检查输入

// 窗口宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器glsl
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//片段着色器glsl
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
	
	//--------初始化窗口---------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	//-------创建窗口对象--------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mytriangle", NULL, NULL);//窗口数据
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();//若创建失败，关闭窗口
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册函数，改变窗口大小时调用
	
	//调用opengl函数之前初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	
	//-----------编译着色器-----------
	
	//顶点着色器
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//附加着色器源代码
	glCompileShader(vertexShader);//编译
	//检测着色器编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX";
	}
	
	//片段着色器
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建着色器对象
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//附加着色器源代码
	glCompileShader(fragmentShader);//编译
	//检测是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILEATION_FAILED\n" << infoLog << endl;
	}
	
	//--------将两个着色器对象链接到一个着色器程序--------
	int shaderProgram = glCreateProgram();//创建着色器程序
	glAttachShader(shaderProgram, vertexShader);//将顶点着色器附加到着色器程序
	glAttachShader(shaderProgram, fragmentShader);//将片段着色器附加到着色器程序
	glLinkProgram(shaderProgram);//链接所有附加着色器
    //检测链接错误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//顶点数组
	float vertices[] = {
		-0.5f,-0.5f,0.0f,//左顶点
		 0.5f,-0.5f,0.0f,//右顶点
		 0.0f, 0.5f,0.0f //上顶点
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);//顶点数组对象
	
	glGenBuffers(1, &VBO);//缓冲区对象
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	//渲染循环
	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window);

		//背景色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);//交换颜色缓冲区
		glfwPollEvents();//检查是否触发事件
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
