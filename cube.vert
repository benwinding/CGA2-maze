
#version 330

// This shader needs both a projection matrix and a view matrix
// gl_Position is the vertex location in clip coordinates

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec4 a_colour;	

out vec4 colour;

void main(void) {
	
	colour = a_colour;	// We simply pass the colour along to the next stage

	gl_Position = projection_matrix * modelview_matrix * vec4(a_vertex, 1.0);

}
