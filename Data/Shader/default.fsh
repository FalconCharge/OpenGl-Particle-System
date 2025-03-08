uniform sampler2D texture1;

in float Rotation;
in vec4 Color;

out vec4 PixelColor;     // Output Pixel color


void main()
{    
    
    // Get the original point sprite coordinates (range [0, 1])
    vec2 uv = gl_PointCoord - vec2(0.5);

    float cosTheta = cos(Rotation);
    float sinTheta = sin(Rotation);

    vec2 rotatedUV = vec2(
        uv.x * cosTheta - uv.y * sinTheta,
        uv.x * sinTheta + uv.y * cosTheta
    );

    rotatedUV += vec2(0.5);

    vec4 diffuse = texture(texture1, rotatedUV);

    PixelColor = diffuse * Color;

    // Simple checker pattern to visualize rotation
    //float checker = step(0.0, sin(rotatedUV.x * 10.0) * sin(rotatedUV.y * 10.0));
    

}

