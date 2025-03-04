uniform mat4 Proj;
uniform mat4 View;

// Input attributes
in vec4 aPos;      // w is scale
in float aScale;
in float aRotation;

out float Rotation;

void main()
{
    
    vec4 pos = View * Proj * vec4(aPos.xyz, 1.0);
    gl_Position = pos;

    gl_PointSize = (1.0 - pos.z / pos.w) * aPos.w;

    Rotation = aRotation;
}
