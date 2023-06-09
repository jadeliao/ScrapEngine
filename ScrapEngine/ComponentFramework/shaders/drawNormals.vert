#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
} ubo;

layout( push_constant ) uniform ModelMatrixPushConst {
	mat4 normalMatrix;
	mat4 modelMatrix;
} modelMatrixPC;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout (location = 0) out VertexStage {
    vec3 normal;
} vs_out;

void main() {

	mat3 normalMatrix = mat3(modelMatrixPC.normalMatrix); 
	mat4 modelMatrix = modelMatrixPC.modelMatrix;

	gl_Position =  ubo.view * modelMatrix * vec4(inPosition, 1.0); 
    vs_out.normal = mat3(modelMatrixPC.normalMatrix) * inNormal;
}