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

layout(binding = 2) uniform GeometryInfo {
    float vLength;
    vec4 colour;
} geometryUbo;

layout( push_constant ) uniform ModelMatrixPushConst {
	mat4 normalMatrix;
	mat4 modelMatrix;
	float alpha;
} modelMatrixPC;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 vertNormal;
layout (location = 1) out vec3 eyeDir;
layout (location = 2) out vec2 fragTexCoord;
layout (location = 3) out vec3 lightDir[3];


void main() {
	const int lightNum = 3;
	fragTexCoord = inTexCoord;
	mat3 normalMatrix = mat3(modelMatrixPC.normalMatrix); 
	mat4 modelMatrix = modelMatrixPC.modelMatrix;
	vertNormal = normalize(normalMatrix * inNormal.xyz); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(ubo.view * modelMatrix * vec4(inPosition, 1.0)); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	for (int i = 0; i < lightNum; i++){
		/// Create the light direction. I do the math with in class 
		lightDir[i] = normalize(vec3(glightUbo.position[i]) - vertPos); 

	}
	
	gl_Position =  ubo.proj * ubo.view * modelMatrix * vec4(inPosition, 1.0); 
}
