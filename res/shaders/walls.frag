#version 330

in vec2 st;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texMap;
uniform vec3 lightPos;

out vec4 fragColour;

void main(void)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    
    vec3 lightColor = vec3(0.8, 0.2, 0.1);
    vec3 objectColor = vec3(0.5, 0.5, 0.5);
    vec3 ambient = vec3(0.9, 0.9, 0.9);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse) * objectColor;
    vec4 colour = vec4(result, 1.0);
    // Modulate (multiply) a preset colour with the texture colour
    // at this location.
    fragColour = colour * texture(texMap, st);	
}
