#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light{
	vec3 position;
	vec3 direction;//�۹�ķ�������
	float cutOff;//�۹�����н�
	float outerCutOff;//�۹�����н�

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//��˥���Ŀ���ϵ��
	float constant;
	float linear;
	float quadratic;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){
	
	vec3 ambient = light.ambient*texture(material.diffuse,Texcoords).rgb;

	
	
	//�����Դ��ƬԴλ�õķ�������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * light.diffuse * texture(material.diffuse,Texcoords).rgb;


	//����߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);//32ָ���Ƿ����
	vec3 specular = spec * light.specular *texture(material.specular,Texcoords).rgb;

	//spotlight
	float theta = dot(lightDir,-light.direction);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon,0,1);


	//attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear *distance + light.quadratic*(distance*distance));

	ambient *= attenuation;
	//specular *= attenuation;
	//diffuse *= attenuation;
	specular *= intensity;
	diffuse *= intensity;


	vec3 result =(ambient + diffuse + specular);

	FragColor = vec4(result,1.0f);

}