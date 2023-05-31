#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in; /// bringing in triangles

layout (line_strip, max_vertices = 3) out; 

layout(binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
} ubo;


layout (location = 0) in VertexStage {
    vec3 normal;
} vs_in[];

layout (location = 1) in vec2 fragTexCoord[];

layout (location = 1) out vec2 TexCoord;


void main() {

    float length  = 0.005f;
    vec4 offset = {2.0, 2.0, 2.0, 2.0};
    for(int index = 0; index < 3; index++){
        gl_Position = gl_in[index].gl_Position + (vec4(vs_in[index].normal, 1.0) * length * index);
        TexCoord = fragTexCoord[index];
        EmitVertex();
    }
    EndPrimitive();
}