#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform float ambientStrength;// 环境光强度
uniform float diffuseStrength; // 漫反射强度
uniform float specularStrength;// 镜面反射强度
uniform int shininess; //反光度
uniform vec3 lightPos; 
uniform vec3 lightColor; // 光照颜色
uniform vec3 objectColor;// 物体颜色
uniform vec3 viewPos;

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

	vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}