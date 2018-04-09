
#version 330

in vec4 pos; // Interpolated vertex position, in object coords

// The final colour we will see at this location on screen
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

	// We will ignore vertex colour this time
	// Draw the top of the cube a different colour to the rest
	
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
