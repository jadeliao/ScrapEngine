#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in; /// bringing in triangles

layout (line_strip, max_vertices = 2) out; 

layout(binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 2) uniform GeometryInfo {
    float vLength;
    vec4 colour;
} geometryUbo;

layout (location = 0) in VertexStage {
    vec3 normal;
} vs_in[];


void main() {
    float length_ = geometryUbo.vLength;
    vec4 offset = {2.0, 2.0, 2.0, 2.0};
    for(int index = 0; index < 2; index++){
        gl_Position = ubo.proj * gl_in[index].gl_Position; 
        EmitVertex();

        gl_Position = ubo.proj * gl_in[index].gl_Position + (vec4(vs_in[index].normal, 1.0) * length_);
        EmitVertex();
        EndPrimitive();
    }

}