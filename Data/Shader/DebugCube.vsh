uniform mat4 MVP;  // Model-View-Projection matrix

// Input attributes
in vec3 aPos;      // Position attribute (3D position)
in vec2 u_uv1;

out vec2 v_uv1;
void main()
{
    vec4 pos = MVP * vec4(aPos, 1.0);
    gl_Position = pos;
    v_uv1 = u_uv1;

}
