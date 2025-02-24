uniform mat4 MVP;  // Model-View-Projection matrix

uniform float uScale;
uniform float vScale;

// Input attributes
in vec3 aPos;      // Position attribute (3D position)
in vec2 a_uv1;


out vec2 v_uv1;


void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    v_uv1 = a_uv1 * vec2(uScale, vScale);
}
