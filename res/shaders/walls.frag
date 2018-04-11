#version 330

in vec2 st;

uniform sampler2D texMap;

out vec4 fragColour;

void main(void)
{
    // Try changing this colour...
    vec4 colour = vec4(1.0, 1.0, 1.0, 1.0);
    
    // Modulate (multiply) a preset colour with the texture colour
    // at this location.
	fragColour = colour * texture(texMap, st);	
}
