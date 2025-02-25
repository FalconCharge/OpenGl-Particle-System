
uniform vec4 color;
uniform sampler2D texture1;  // Texture sampler

out vec4 PixelColor;     // Output Pixel color

void main()
{
    //Applys the texure to the point
    vec4 diffuse = texture(texture1, gl_PointCoord);

    if(length(diffuse.rgb) < 0.35) discard;

    PixelColor = diffuse * color;
}

