#version 330 core
layout (location = 0) in vec3 position; // 数据位0 顶点位置
layout (location = 1) in vec3 normal;   // 数据位1 法线向量

out vec3 Normal;         // 法向量
out vec3 FragPos;        // 片段位置

uniform mat4 model;      // 模型矩阵
uniform mat4 view;       // 视图矩阵
uniform mat4 projection; // 投影矩阵

void main()
{
	// 顶点矩阵计算
    gl_Position = projection * view *  model * vec4(position, 1.0f);
	// 片段位置计算
    FragPos = vec3(model * vec4(position, 1.0f));
	// 法线向量矩阵变换计算
	// 将模型矩阵 正规矩阵化 ,然后乘以法线向量
	// 使法线向量可以随物体一起进行矩阵变换
    Normal = mat3(transpose(inverse(model))) * normal;  
} 
