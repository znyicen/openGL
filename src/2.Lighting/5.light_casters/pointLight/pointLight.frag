#version 330 core

// 材质属性
struct Material
{
	sampler2D diffuse;   // 用漫射纹理代替diffuse颜色
	sampler2D specular;  // 镜面高光
	float     shininess; // 高光强度
}; 

// 灯光属性
struct Light
{
	vec3 position;   // 灯光位置
	
	vec3 ambient;    // 环境光
	vec3 diffuse;    // 反射光
	vec3 specular;   // 高光
	
	float constant;  // 常数
	float linear;    // 线性衰减
	float quadratic; // 二次方衰减
};


in vec3 FragPos;           // 当前片段（或像素）位置
in vec3 Normal;            // 法向量
in vec2 TexCoords;         // 漫反射贴图纹理坐标

out vec4 color;            // 片段着色器最终输出颜色
  
uniform vec3 viewPos;      // 观察者坐标
uniform Material material; // 材质属性
uniform Light light;       // 灯光属性

void main()
{
	// 纹理采样
	vec3 diffTex = vec3(texture(material.diffuse, TexCoords));
	vec3 specTex = vec3(texture(material.specular, TexCoords));
	

	// Ambient 环境色（很少有完全黑暗的场景）
	vec3 ambient = light.ambient * diffTex;                     // 环境因子计算

	// Diffuse 漫反射
	vec3 norm = normalize(Normal);                              // 标准化法线
	vec3 lightDir = normalize(light.position - FragPos);        // 计算灯光方向
	// vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);	                // 计算散射光强度  
	vec3 diffuse = light.diffuse * diff * diffTex;              // 得到散射因子

	// Specular 高光
	vec3 viewDir = normalize(viewPos - FragPos);                // 计算视线方向
	vec3 reflectDir = reflect(-lightDir, norm);                 // 计算反射坐标
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算高光范围
	vec3 specular = light.specular * spec * specTex;            // 高光因子
	
	// 计算灯光衰减值
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0f / 
		(light.constant + light.linear*distance + light.quadratic*(distance*distance));
		
	// 衰减灯光
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	color = vec4(ambient + diffuse + specular, 1.0f);
} 
