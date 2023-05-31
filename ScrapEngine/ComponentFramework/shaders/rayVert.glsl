#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;

layout(std140, binding = 0) uniform CameraMatricies{
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 1) uniform LightData{
    vec4 position[3];
    vec4 colour[3];
};

layout(location = 2) uniform mat4 modelMatrix;

void main() {
    gl_Position = projection * view * vec4(vVertex.xyz, 1.0);
}