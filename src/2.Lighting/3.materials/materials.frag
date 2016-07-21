#version 330 core

// 材质属性
struct Material      // 创建一个结构体
{
	vec3 ambient;    // 环境
	vec3 diffuse;    // 漫反射
	vec3 specular;   // 镜面高光
	float shininess; // 高光强度
}; 

// 灯光属性
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 FragPos;           // 当前片段（或像素）位置
in vec3 Normal;            // 法向量

out vec4 color;            // 片段着色器最终输出颜色
  
uniform vec3 viewPos;      // 观察者坐标
uniform Material material; // 材质属性
uniform Light light;       // 灯光属性

void main()
{
	// 在标准化的颜色的体系下0.0f-1.0f。

    // Ambient 环境色（很少有完全黑暗的场景）
    vec3 ambient = light.ambient * material.ambient;		    // 环境因子计算
  	
    // Diffuse 漫反射
    vec3 norm = normalize(Normal);								// 标准化法线
    vec3 lightDir = normalize(light.position - FragPos);	    // 计算灯光方向
    float diff = max(dot(norm, lightDir), 0.0);					// 计算散射光强度  
    vec3 diffuse = light.diffuse * (diff * material.diffuse);   // 得到散射因子
    
    // Specular 高光
    vec3 viewDir = normalize(viewPos - FragPos);                // 计算视线方向
    vec3 reflectDir = reflect(-lightDir, norm);                 // 计算反射坐标
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算高光范围
    vec3 specular = light.specular * (spec * material.specular);// 高光因子
        
    vec3 result = ambient + diffuse + specular;                 // 最后计算最终的片段颜色输出
    color = vec4(result, 1.0f);
} 
