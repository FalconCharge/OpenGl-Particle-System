uniform mat4 MVP;  // Model-View-Projection matrix
uniform float size;

// Input attributes
in vec3 aPos;      // Position attribute (3D position)

void main()
{
    vec4 pos = MVP * vec4(aPos, 1.0);
    gl_Position = pos;

    gl_PointSize = (1.0 - pos.z / pos.w) * size;

}
