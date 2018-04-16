
#version 330

layout (location = 0) in vec3 a_vertex; 
layout (location = 1) in vec3 a_normal; 
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec2 scale;

out vec4 pos;
out vec2 st;

void main(void)
{
	st = a_tex_coord * scale;
	pos = vec4(a_vertex, 1.0f);

	gl_Position = projection * view * model * vec4(a_vertex, 1.0);
}
