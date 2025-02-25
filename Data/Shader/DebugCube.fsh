
uniform vec4 color;
uniform sampler2D texture1;  // Texture sampler

in vec2 v_uv1;

out vec4 PixelColor;     // Output Pixel color

void main()
{


    PixelColor = color;
}

