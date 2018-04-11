
#version 330

// Position and colour variables. 1 per vertex.
layout (location = 0) in vec3 a_vertex; 
//layout (location = 1) in vec4 a_colour;	

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec2 a_tex_coord;

out vec2 st;

void main(void)
{
	st = a_tex_coord;

    // a_vertex is vertex position in object coordinates
    // gl_position is vertex position in camera coordinates
	gl_Position = projection * view * model * vec4(a_vertex, 1.0);
}
