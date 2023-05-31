#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) out vec4 fragColor;

layout (location = 0) in VertexStage {
    vec3 normal;
} vs_out;

layout (location = 2) in vec2 fragTexCoord;

void main() {
    fragColor =  texture(texSampler, fragTexCoord);
}