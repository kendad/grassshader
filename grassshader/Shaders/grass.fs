#version 330 core

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;

//Attanuation
uniform float constant;
uniform float linear;
uniform float quadratic;

//Spotlight
uniform vec3 spotlightDirection;
uniform float cutOff;
uniform float outerCutOff;

in fData{
	vec3 FragPos;
	vec3 color;
	vec3 normal;
}frag_in;

const vec3 LIGHTCOLOR=vec3(1.0f,1.0f,1.0f);

void main()
{   //Attanuation
	float distance=length(lightPos-frag_in.FragPos);
	float attenuation=1.0f/(constant+(linear*distance)+(quadratic*(distance*distance)));
	attenuation=attenuation*1;
	
	//Ambient
	float ambientStrength=0.1f;
	vec3 ambient=ambientStrength*frag_in.color*LIGHTCOLOR*attenuation;
	
	//Diffuse
	vec3 norm=normalize(frag_in.normal);
	vec3 lightDir=normalize(lightPos-frag_in.FragPos);
	float diffuseStrength=max(dot(norm,lightDir),0.0f);
	vec3 diffuse=diffuseStrength*frag_in.color*LIGHTCOLOR*attenuation;
	
	//Specular
	float specularStrength=0.1f;
	vec3 viewDir=normalize(cameraPos-frag_in.FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(cameraPos,reflectDir),0.0),2);
	vec3 specular=specularStrength*spec*frag_in.color*LIGHTCOLOR*attenuation;
	
	//Spotlight
	float theta=dot(lightDir,normalize(-spotlightDirection));
	float epsilon=cutOff-outerCutOff;
	float intensitySpotlight=clamp((theta-outerCutOff)/epsilon,0.0f,1.0f);
	//diffuse*=intensitySpotlight;
	//specular*=intensitySpotlight;
	
	//Resultant Color
	vec3 result=ambient+diffuse+specular;

	FragColor=vec4(result,1.0f);
}