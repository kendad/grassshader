#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float time;

const float windStrength=0.04f;
const float grassLength=0.25f;

in vData{
	vec3 normal;
}vertices[];

out fData{
	vec3 FragPos;
	vec3 color;
	vec3 normal;
}frag;

void main(){
	
	vec3 position=gl_in[0].gl_Position.xyz;
	
	frag.color=vec3(0.0f,0.390625f,0.0f);
	//frag.FragPos=vec3(gl_in[0].gl_Position);
	frag.normal=vertices[0].normal;
	gl_Position=gl_in[0].gl_Position;
	frag.FragPos=vec3(gl_in[0].gl_Position);
	EmitVertex();
	
	frag.color=vec3(0.0f,0.390625f,0.0f);
	//frag.FragPos=vec3(gl_in[0].gl_Position+vec4(0.02f,0.0f,0.0f,0.0f)).xyz;
	frag.normal=vertices[0].normal;
	gl_Position=gl_in[0].gl_Position+vec4(0.02f,0.0f,0.0f,0.0f);
	frag.FragPos=vec3(gl_in[0].gl_Position+vec4(0.02f,0.0f,0.0f,0.0f));
	EmitVertex();
    
	//wind calculations
	//vec3 wind=vec3(sin(time+position.x+position.z*1000)*windStrength,0.4f,cos(time+position.x+position.z)*windStrength);
	//vec3 wind=vec3((sin(time+position.x)+sin(time+position.z*2))*windStrength,grassLength,(cos(time+position.x*2)+cos(time+position.z))*(windStrength-0.06f));
	vec3 wind=vec3((sin(time+position.x)+sin(time+position.z*2))*windStrength+cos(time+position.z)*windStrength,grassLength,0.0f);
	
	frag.color=vec3(0.5f,1.0f,0.0f);
	//frag.FragPos=vec3(gl_in[0].gl_Position + vec4(wind,0.0f));
	frag.normal=vertices[0].normal;
	gl_Position=gl_in[0].gl_Position + vec4(wind,0.0f);
	frag.FragPos=vec3(gl_in[0].gl_Position + vec4(wind,0.0f));
	EmitVertex();
	
	EndPrimitive();
}