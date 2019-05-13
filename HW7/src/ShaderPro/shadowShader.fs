#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform float ambientStrength;// ������ǿ��
uniform float diffuseStrength; // ������ǿ��
uniform float specularStrength;// ���淴��ǿ��
uniform int shininess; //�����
uniform vec3 lightColor; // ������ɫ
uniform vec3 objectColor;// ������ɫ

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// ִ��͸�ӳ���
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // ȡ�õ�ǰƬԪ�ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
	vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
	//�޸���Ӱʧ��
	//float bias = 0.005;
	float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.005);
	// ��鵱ǰƬԪ�Ƿ�����Ӱ�� 
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
    //ֻҪͶӰ������z�������1.0���Ͱ�shadow��ֵǿ����Ϊ0.0
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

	// ������Ӱ
    float shadow = ShadowCalculation(FragPosLightSpace);       
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * objectColor;    

    FragColor = vec4(result, 1.0);
}