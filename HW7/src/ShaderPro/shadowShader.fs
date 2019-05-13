#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform float ambientStrength;// 环境光强度
uniform float diffuseStrength; // 漫反射强度
uniform float specularStrength;// 镜面反射强度
uniform int shininess; //反光度
uniform vec3 lightColor; // 光照颜色
uniform vec3 objectColor;// 物体颜色

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
	vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
	//修复阴影失真
	//float bias = 0.005;
	float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.005);
	// 检查当前片元是否在阴影中 
	//float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	//PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    //只要投影向量的z坐标大于1.0，就把shadow的值强制设为0.0
    if(projCoords.z > 1.0)
        shadow = 0.0; 
	return shadow;
}

void main() 
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	// 计算阴影
    float shadow = ShadowCalculation(FragPosLightSpace);       
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * objectColor;    

    FragColor = vec4(result, 1.0);
}