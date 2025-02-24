in vec3 v_pos;
in vec2 v_uv1;
in vec3 v_normal;

uniform sampler2D tex;  // The texture
uniform vec3 lightDirection;       // Direction of the directional light
uniform vec3 lightColor;           // Color of the directional light
uniform float ambientStrength;     // Ambient lighting strength

uniform vec4 color;

out vec4 FragColor;

void main() {

    //FragColor = vec4(lightColor, 1.0f); outputs the color of the light

    // Normalize the normal vector
    vec3 normal = normalize(v_normal);

    // Compute the diffuse lighting (dot product between light direction and normal)
    float diff = max(dot(normal, -lightDirection), 0.0);

    // Ambient lighting (you can add more here)
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 diffuse = diff * lightColor;

    // Calculate the final color
    //vec4 texColor = texture(tex, v_uv1);

    // Object color
    vec4 objectColor = color;

    // Final color (ambient + diffuse)
    vec3 finalColor = (ambient + diffuse) * color.rgb;

    FragColor = vec4(finalColor, color.a);
    
}
