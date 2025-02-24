in vec2 v_uv1;        

uniform sampler2D tex;  // Texture sampler

out vec4 PixelColor;     // Output Pixel color

void main()
{
    PixelColor = texture(tex, v_uv1);
}

