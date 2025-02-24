uniform mat4 MVP;  // Model-View-Projection matrix

// Input attributes
in vec3 aPos;      // Position attribute (3D position)


void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
}
