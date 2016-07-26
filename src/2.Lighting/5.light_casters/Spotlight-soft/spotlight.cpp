// 流处理
#include <iostream> 
#include <cmath>

// GLEW
// GLEW 能自动识别你的平台所支持的全部OpenGL高级扩展涵数
#define GLEW_STATIC 
#include <GL/glew.h>

// GLFW
// OpenGL 窗口管理、分辨率切换、键盘、鼠标以及游戏手柄、定时器输入、线程创建等等。
#include <GLFW/glfw3.h>

// Other Libs
// 图像读取库
#include <SOIL.h>
// GLM 数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include <learnopengl/shader.h>
#include <learnopengl/filesystem.h>
#include <learnopengl/Camera.h>

// 函数原型
// 7.1
// 键盘输入回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// 鼠标输入回调函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚动回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 输入处理函数
void do_movement();

// 窗口尺寸
const GLuint WIDTH = 800, HEIGHT = 600;

// 摄影机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH  / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// 灯光属性
glm::vec3 lightPos(1.2f, 0.5f, 2.0f);

// 帧耗时
GLfloat deltaTime = 0.0f; // 当前帧到上一帧所耗时间
GLfloat lastFrame = 0.0f; // 上一帧

// 主程序
int main() {
	// 1.0初始化GLFW
	glfwInit();
	// 1.1设置GLFW基本参数
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 2.0创建窗口对象
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	// 2.1加入openGL的上下文中。。。
	glfwMakeContextCurrent(window);

	// 7.2注册回调函数
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	// 不显示光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 3.0初始化GLEW
	// 多使用现代化技术
	glewExperimental = GL_TRUE; 
	glewInit();

	// 4.0视口配置
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 5.0构建和编译着色器（外部文件链接）
	// 参数为文件路径: vertexPath, fragmentPath, geometryPath
	// shader.h的Shader类的构造函数
	Shader lightingShader("light_casters.vs", "light_casters.frag"); 
	Shader lampShader("lamp.vs", "lamp.frag");

	// *本例没有使用color顶点属性及EBO
	// 6.0设置顶点数据
	GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

	// Positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

	// #1
	// 顶点缓存对象，顶点数组对象
	GLuint VBO, containerVAO;   
	// 生成各自缓存ID
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	// #2 把VBO绑定到GL_ARRAY_BUFFER(openGL顶点缓存对象）上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// #3 将顶点数据复制到缓冲的内存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 使用VAO要做的全部就是绑定VAO
	glBindVertexArray(containerVAO);

	// #4 解释顶点数据 （数据位置，数据位数，数据格式，？，步长，偏移量）
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)
		(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 纹理坐标属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
	// #5 解绑
	glBindVertexArray(0);


	// **为灯创建一个VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为容器(物体)的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性指针(仅设置灯的顶点数据)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	// ~新建纹理单元~
	// 新建纹理
	GLuint diffuseMap, specularMap;  
	glGenTextures(1, &diffuseMap); // 生成纹理的数量，然后将数据存储在GLuint数组中
	glGenTextures(1, &specularMap);

	// 纹理共用变量
	int width, height;    // 图片宽，高
	unsigned char* image; // 装载纹理变量

	// ~漫射贴图~
	// 读取纹理
	image = SOIL_load_image(FileSystem::getPath(
		"resources/textures/container2.png").c_str(), &width, &height, 0,
		SOIL_LOAD_RGB);
	
	// 绑定
	glBindTexture(GL_TEXTURE_2D, diffuseMap); 
	// 生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// 生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D); 

	// 生成了纹理和相应的多级渐远纹理后，解绑纹理对象、释放图像的内存很重要。
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
	 
	// 设置贴图环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置纹理过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// ~镜面贴图~
    image = SOIL_load_image(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
	// ~获取Uniform对象，向着色器传递贴图~
	// 激活对象照明着色器，灯光为lampShader
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"),  0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);


	// 7.0主循环
	while (!glfwWindowShouldClose(window))
	{
		// 就算当前帧的deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 7.1检测事件
		glfwPollEvents();
		// 按键处理
		do_movement();

		// 渲染
		// 7.2清空颜色缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除深度缓冲区

		// 激活对象照明着色器，灯光为lampShader
		lightingShader.Use();

		// 设置视角位置地址
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);

		// 设置聚光灯属性（聚光灯朝向使用摄影机镜头朝向）
		GLint lightPosLoc             = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint lightSpotDirPos         = glGetUniformLocation(lightingShader.Program, "light.direction");
		GLint lightSpotCutOffLoc      = glGetUniformLocation(lightingShader.Program, "light.cutOff");
		GLint lightSpotOuterCutOffLoc = glGetUniformLocation(lightingShader.Program, "light.outerCutOff");
		glUniform3f(lightPosLoc,             camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightSpotDirPos,         camera.Front.x, camera.Front.y, camera.Front.z);
		// 提前计算余弦值，是为了节约开销。
		// 在片段着色器中，我们计算LightDir和SpotDir向量的点乘，而点乘返回一个余弦值，
		// 不是一个角度，所以我们不能直接把一个角度和余弦值对比。
		glUniform1f(lightSpotCutOffLoc,      glm::cos(glm::radians(12.5f)));
		glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(17.5f)));

        
		// 设置光照属性
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"),  0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"),  0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		// 灯光衰减属性
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.constant"),  1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.linear"),    0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "light.quadratic"), 0.032f);

        // 设置材质属性
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// 7.5创建摄影机/视角变换
		// 定义视角（观察者）矩阵
		glm::mat4 view; 
		// 使用摄像机类创建观察矩阵,并将其传递给视图矩阵
		view = camera.GetViewMatrix();
		// 创建投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// 获取各个矩阵在着色器的uniform的地址
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc  = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc  = glGetUniformLocation(lightingShader.Program, "projection");

		// 把矩阵传递给着色器 
		// 参数1：uniform的地址(Location)
		// 参数2：将要发送多少个矩阵，目前是1
		// 参数3：询问我们我们是否希望对我们的矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列。
		// OpenGL开发者通常使用一种内部矩阵布局叫做以列为主顺序的(Column-major Ordering)布局。
		// GLM已经是用以列为主顺序定义了它的矩阵，所以并不需要置换矩阵，我们填GL_FALSE。
		// 参数4：是实际的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望的那种，
		// 因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 激活漫反射贴图
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// 激活镜面贴图
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

		// 7.6绘制图形
		glm::mat4 model; 
		glBindVertexArray(containerVAO); // 绑VAO
		for (GLuint i = 0; i < 10; i++)
        {
			model = glm::mat4(); // 清空模型矩阵
			
			// 平移
			model = glm::translate(model, cubePositions[i]); // 引入早已定义好的空间位置
			
			// 旋转（欧拉角）
			// 在3D空间中旋转需要一个角(angle)和一个旋转轴(Rotation Axis)。
			GLfloat angle = 20.0f * i;                    // 角位移
			glm::vec3 axis = glm::vec3(1.0f, 0.3f, 0.5f); // 旋转轴
			model = glm::rotate(model, angle, axis);      // 将模型矩阵绕axis轴旋转angle度
			
			// 将矩阵传入着色器
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// ~绘制箱子~
			glDrawArrays(GL_TRIANGLES, 0, 36); // 共36个顶点
		}
		glBindVertexArray(0); // 解绑

		// 激活灯光着色器
		lampShader.Use();

		// 处理 Uniform
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc  = glGetUniformLocation(lampShader.Program, "view");
		projLoc  = glGetUniformLocation(lampShader.Program, "projection");
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 模型矩阵
		model = glm::mat4();
		// 平移
		model = glm::translate(model, lightPos);
		// 缩放
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// ~绘制灯光对象~
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// 7.7交换缓冲区
		glfwSwapBuffers(window);
	}
	// 8.0释放资源
	glDeleteVertexArrays(1, &containerVAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;
}

// 输入回调函数实现
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// 键位状态记录(待住函数中一并处理，则可支持多键位同事处理）
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// 键位状态处理
void do_movement()
{
	// 摄像机控制
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05; // 鼠标灵敏度
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
