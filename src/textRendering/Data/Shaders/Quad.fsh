
in vec2 v_uv1;          // Input texture coordinates from the vertex shader

uniform sampler2D tex; // The texture to sample
uniform vec4 color;

out vec4 PixelColor;     // Output Pixel color

void main()
{
    vec4 pixel = texture(tex, v_uv1);

    
    if (length(pixel.rgb) < 0.4f) {
        pixel.a = 0.0f; //Set this pixels aplha to 0
    }else {
        // Replace non-black areas with white
        pixel.rgb = vec3(1.0, 1.0, 1.0);
    }
    

    PixelColor = vec4(pixel.rgb * color.rgb, pixel.a * color.a);
}

