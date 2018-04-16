#version 330

in vec4 pos; // Interpolated vertex position, in object coords 
in vec2 st;

uniform sampler2D texMap;

out vec4 fragColour;

void main(void)
{
	vec4 s0 = vec4(0.5, 0.5, 0.5, 1.0); 
	vec4 s1 = vec4(0.1, 0.6, 0.8, 1.0); 
	vec4 s2 = vec4(0.2, 0.7, 0.8, 1.0); 
	vec4 s3 = vec4(0.3, 0.7, 0.6, 1.0); 
	vec4 s4 = vec4(0.4, 0.9, 0.6, 1.0); 
	vec4 s5 = vec4(0.5, 0.9, 0.1, 1.0); 
	vec4 s6 = vec4(0.6, 0.6, 0.1, 1.0); 

    vec4 colour = vec4(0.4f, 0.4f, 0.4f, 0.5);

    // Modulate (multiply) a preset colour with the texture colour
    // at this location.
    fragColour = colour + texture(texMap, st);	

	if (pos.x >= 0.99) { 
		fragColour = s1; 
	} 
	else if (pos.x <= -0.99) { 
		fragColour = s2; 
	} 
	else if (pos.y >= 0.99) { 
		fragColour = s3; 
	} 
	else if (pos.y <= -0.99) { 
		fragColour = s4; 
	} 
	else if (pos.z >= 0.99) { 
		fragColour = s5; 
	} 
	else if (pos.z <= -0.99) { 
		fragColour = s6; 
	} 
	else { 
		fragColour = s1; 
	} 
}
