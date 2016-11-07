// 片段着色器 Fragment Shader
#version 330 core
in vec3 ourColor;					// 接受颜色信息

out vec4 color;						// 输出计算好的颜色信息

void main()
{
    color = vec4(ourColor, 1.0f);	// vec4 为alha值
}
