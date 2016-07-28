#version 330 core

// 结构体定义
struct Material {       // 材质属性
    sampler2D diffuse;  // 漫反射贴图（通常就是物体本来的贴图）
    sampler2D specular; // 高光（镜面）贴图
    float shininess;    // 高光的范围（幂）
}; 

struct DirLight {       // 平行光属性
    vec3 direction;     // 灯光的方向
	// 对物体光照的各项上限值
    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射光
    vec3 specular;      // 高光
};

struct PointLight {     // 点光源属性
    vec3 position;      // 灯光的位置
    
    float constant;     // 衰减的常数
    float linear;       // 线性衰减值
    float quadratic;    // 二次方衰减值

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {      // 聚光灯属性
    vec3 position;
    vec3 direction;

    float cutOff;       // 内部光切角
    float outerCutOff;  // 外部光切角
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

// 定义点光源的数量
#define NR_POINT_LIGHTS 4

in vec3 FragPos;      // 本片段的位置
in vec3 Normal;       // 法向量
in vec2 TexCoords;    // 贴图坐标

out vec4 color;       // 最终输出颜色

uniform vec3 viewPos; // 视角（观察者）位置
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// 函数原型-灯光照射彩度计算
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // 属性计算
    vec3 norm = normalize(Normal); // 因为顶点数据间的片段会进行插值，所以需要标准化法向量
    vec3 viewDir = normalize(viewPos - FragPos); // 计算视角指向
    
    // == ======================================
    // 分三阶段计算光照，最后将结果合并。
    // == ======================================
    // 阶段 1: 平行光
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 阶段 2: 点光
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // 阶段 3: 聚光灯
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    color = vec4(result, 1.0);
}

// 计算平行光的光照彩度
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// 计算点光的光照彩度
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算灯光衰减系数
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// 计算聚光灯的光照彩度
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算灯光衰减系数
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 聚光灯强度(边缘模糊)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
