#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 proj;
} ubo;

layout(binding = 1) uniform GlobalLight {
    vec4 position[3];
    vec4 diffuse[3];
} glightUbo;

layout( push_constant ) uniform ModelMatrixPushConst {
	mat4 normalMatrix;
	mat4 modelMatrix;
} modelMatrixPC;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout (location = 0) out VertexStage {
    vec3 normal;
} vs_out;

layout (location = 1) out vec2 fragTexCoord;

void main() {
	fragTexCoord = inTexCoord;
	mat3 normalMatrix = mat3(modelMatrixPC.normalMatrix); 
	mat4 modelMatrix = modelMatrixPC.modelMatrix;
	vs_out.normal = normalize(normalMatrix * inNormal.xyz); /// Rotate the normal to the correct orientation 
	gl_Position =  ubo.proj * ubo.view * modelMatrix * vec4(inPosition, 1.0); 
	
}