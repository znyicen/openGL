#version 330 core
out vec4 color;           // 片段着色器最终输出颜色

in vec3 FragPos;          // 当前片段（或像素）位置
in vec3 Normal;           // 法向量
  
uniform vec3 lightPos;    // 灯光坐标
uniform vec3 viewPos;     // 观察者坐标
uniform vec3 lightColor;  // 灯光颜色
uniform vec3 objectColor; // 物体固有色

void main()
{
	// 在标准化的颜色的体系下0.0f-1.0f。

    // Ambient 环境色（很少有完全黑暗的场景）
    float ambientStrength = 0.1f;                  // 环境光强度
    vec3 ambient = ambientStrength * lightColor;   // 环境因子计算
  	
    // Diffuse 漫反射
    vec3 norm = normalize(Normal);                 // 标准化法线
    vec3 lightDir = normalize(lightPos - FragPos); // 计算灯光方向
    float diff = max(dot(norm, lightDir), 0.0);    // 计算散射光强度  
    vec3 diffuse = diff * lightColor;              // 得到散射因子
    
    // Specular 高光
    float specularStrength = 0.5f;                 // 高光强度
    vec3 viewDir = normalize(viewPos - FragPos);   // 计算视线方向
    vec3 reflectDir = reflect(-lightDir, norm);    // 计算反射坐标
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 计算高光范围
    vec3 specular = specularStrength * spec * lightColor;     // 高光因子
        
    vec3 result = (ambient + diffuse + specular) * objectColor; // 最后计算最终的片段颜色输出
    color = vec4(result, 1.0f);
} 
