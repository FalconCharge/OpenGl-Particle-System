uniform mat4 Proj;
uniform mat4 View;

uniform float size;

// Input attributes
in vec3 aPos;      // Wolrd Space Position

void main()
{
    vec4 pos = View * Proj * vec4(aPos, 1.0);
    gl_Position = pos;

    gl_PointSize = (1.0 - pos.z / pos.w) * size;
}
