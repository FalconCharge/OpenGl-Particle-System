in vec3 a_pos;
in vec2 a_uv1;
in vec3 a_normal;

uniform mat4 MVP;
uniform mat4 worldIT;
uniform mat4 world;

out vec4 v_pos;
out vec2 v_uv1;
out vec3 v_normal;

void main() {
    // Setting vertex position
    gl_Position = MVP * vec4(a_pos, 1.0);

    // Passing pos, tex cords, normals to fsh
    v_pos = (world * vec4(a_pos, 1.0));

    v_uv1 = a_uv1;
    v_normal = (worldIT * vec4(a_normal, 0.0)).xyz;
}
