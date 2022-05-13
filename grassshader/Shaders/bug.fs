#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform float time;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);
	FragColor=vec4(1.0f,1.0f,clamp(sin(time),0.3,0.7),1.0f);
}