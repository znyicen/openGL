//// I 库头 ////
#include <iostream>

// GLEW
// GLEW 能自动识别你的平台所支持的全部OpenGL高级扩展涵数
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
// OpenGL 窗口管理、分辨率切换、键盘、鼠标以及游戏手柄、定时器输入、线程创建等等。
#include <GLFW/glfw3.h>

// Other includes
// 着色器外部编译类库
#include <learnopengl/shader.h> 


//// II 函数原型 ////
void key_callback(GLFWwindow* window, int key, int scancode,
	int action, int mode);

//// III 全局变量 ////
const GLuint WIDTH = 800, HEIGHT = 600;

//// IV 主程序 ////
int main()
{
	std::cout << "进入GLFW环境, OpenGL版本：3.3。" << std::endl;
	// 1.0 初始化GLFW
	glfwInit();
	// 1.1 设定GLFW程序基本模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 2.0 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
		"LearnOpenGL", nullptr, nullptr);
	// 2.1 将窗口加入openGL环境
	glfwMakeContextCurrent(window);

	// 使用自定义函数
	glfwSetKeyCallback(window, key_callback);

	// 3.0 初始化GLEW
	glewExperimental = GL_TRUE; // 使用现代化技术
	glewInit();

	// 4.0 设置视口
	glViewport(0, 0, WIDTH, HEIGHT);


	// 5.0 编写并编译着色器程序
	Shader ourShader("basic.vert", "basic.frag");


	// 6.0 设置顶点数据
	GLfloat vertices[] = {
		 // Positions         // Colors
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
		 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top 
	};

	// 6.1 创建数据对象
	GLuint VBO, VAO;			// 顶点缓存对象，顶点数组对象
	glGenVertexArrays(1, &VAO);	// 生成缓存ID
	glGenBuffers(1, &VBO);		// 生成缓存ID
	glBindVertexArray(VAO);		// 使用VAO要做的全部就是绑定VAO

	// 6.2 把VBO绑定到GL_ARRAY_BUFFER(openGL顶点缓存对象)上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 6.3 将顶点数据复制导缓冲的内存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 6.4 解释顶点数据（数据的：位置、数位、格式，？，步长，偏移量）
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// 6.5 解绑
	glBindVertexArray(0);


	// 7.0 游戏循环
	while (!glfwWindowShouldClose(window))
	{
		// 检测输入事件
		glfwPollEvents();

		// 渲染
		// 清空颜色缓冲区
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 开始绘制
		ourShader.Use();
		glBindVertexArray(VAO); // 绑VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);   // 解绑

		// 交换屏幕缓冲区
		glfwSwapBuffers(window);
	}
	// 8.0 释放资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

//// V 函数实现 ////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
