#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 3) uniform sampler2D texSampler;

layout(binding = 2) uniform GeometryInfo {
    float vLength;
    vec4 colour;
} geometryUbo;

layout(location = 0) out vec4 fragColor;

layout (location = 0) in VertexStage {
    vec3 normal;
} vs_out;

void main() {

    //fragColor = vec4(0.0, 1.0, 0.0, 0.0);

    fragColor = geometryUbo.colour;
}