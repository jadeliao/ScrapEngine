#version 450
#extension GL_ARB_separate_shader_objects : enable

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


layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) out vec4 fragColor;

layout (location = 0) in vec3 vertNormal;
layout (location = 1) in vec3 eyeDir;
layout (location = 2) in vec2 fragTexCoord;
layout (location = 3) in vec3 lightDir[3];


void main() {
	const int lightNum = 3;
	vec4 kd[lightNum] = glightUbo.diffuse;
	vec4 kt = texture(texSampler, fragTexCoord);
	vec4 ks[lightNum];
	for (int i = 0; i < lightNum; i++){
		ks[i] = 0.7 * kd[i];
	}

	vec4 ka = 0.05  * kt;
	vec4 color = ka;

	float diff[lightNum];
	vec3 reflection[lightNum];
	float spec[lightNum];
	for (int i = 0; i < lightNum; i++){
		diff[i] = max(dot(vertNormal, lightDir[i]), 0.0);
		reflection[i] = normalize(reflect(-lightDir[i], vertNormal));
		spec[i] = max(dot(eyeDir, reflection[i]), 0.0);
		spec[i] = pow(spec[i],14.0);
		color += (diff[i] * kt * kd[i]) + (spec[i] * ks[i]);
	}
	
	fragColor = color * modelMatrixPC.alpha;

} 

