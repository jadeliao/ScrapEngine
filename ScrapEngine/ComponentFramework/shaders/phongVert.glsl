#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 uvCoord;

layout(std140, binding = 0) uniform CameraMatricies{
    mat4 projection;
    mat4 view;
};

layout(std140, binding = 1) uniform LightData{
    vec4 position[3];
    vec4 colour[3];
};

layout(location = 2) uniform mat4 modelMatrix;
layout(location = 3) uniform float alpha;

layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 eyeDir; 
layout(location = 2) out vec2 texCoord; 
layout(location = 3) out float textAlpha;
layout(location = 4) out vec3 lightDir[3];


void main() {
	const int lightNum = 3;
    texCoord = uvCoord;
    textAlpha = alpha;
    //textAlpha = 1.0;
    texCoord.y *= -1.0;
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vertNormal = normalize(normalMatrix * vNormal); /// Rotate the normal to the correct orientation 
    vec3 vertPos = vec3(view * modelMatrix * vVertex);
    vec3 vertDir = normalize(vertPos);
    eyeDir = -vertDir;
    for (int i = 0; i < lightNum; i++){
		/// Create the light direction. I do the math with in class 
		lightDir[i] = normalize(vec3(position[i]) - vertPos); 
	}

    gl_Position = projection * view * modelMatrix * vVertex;
}