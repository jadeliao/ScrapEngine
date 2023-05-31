#include "VulkanObject.h"
#include "Debug.h"

VulkanObject::VulkanObject(const char* name_):RenderObject(name_) {
}

VulkanObject::VulkanObject(const char* name_, Matrix4 modelMatrix_): RenderObject(name_) {
	SetModelMatrix(modelMatrix_);

}

VulkanObject::~VulkanObject(){
}

bool VulkanObject::OnCreate(){
	currentTexture = textures.begin()->first;
	return true;
}

void VulkanObject::OnDestroy(){
	for (auto texture : textures) {
		if (texture.second) delete texture.second;
	}
}

void VulkanObject::AddTexture(const char* textureName_, const char* imageName_){
	VulkanMaterial* texture_ = new VulkanMaterial(nullptr, textureName_, imageName_);
	TexturePair* texturePair = new TexturePair();
	texturePair->second = texture_;
	textures[textureName_] = texturePair;
}

TexturePair* VulkanObject::getTexture(){
	return getTexture(currentTexture);
}

TexturePair* VulkanObject::getTexture(const char* textureName_){
	TexturePair* texturePair = nullptr;
	if (textures.empty()) {
		return texturePair;
	}
	if (textures.count(textureName_) > 0) {
		texturePair = textures.find(textureName_)->second;
		return texturePair;
	}
	return texturePair;

}

void VulkanObject::setTexture(const char* textureName_, VulkanMaterial** texture_){
	if (textures.count(textureName_) > 0) {
		textures[textureName_]->second = *texture_;
	}
}

void VulkanObject::SetTexture(const char* texture_){
	if (textures.count(texture_) > 0) {
		currentTexture = texture_;
	}
	else {
		Debug::Info("SetTexture() not a valid texture: ", __FILE__, __LINE__);
	}

}

std::vector<const char*> VulkanObject::getTextureNames(){

	std::vector<const char*> textureNames;

	for (auto texture_ : textures) {
		textureNames.push_back(texture_.first);
	}

	return textureNames;
}

void VulkanObject::LoadModel(const char* modelName_, const char* modelFile_){
	//model = new VulkanMesh(nullptr, modelName_, modelFile_);

}

void VulkanObject::LoadShader(const char* shaderName_, const char* vertFile_, const char* fragFile_){
	//pipeline = new VulkanShader(nullptr, shaderName_, vertFile_, fragFile_);

}

void VulkanObject::NextTexture(){
	//Use iterator to loop through the map
	std::unordered_map<const char*, TexturePair*>::iterator it = textures.begin();
	while (it != textures.end()){
		//Get the next element when find the current texture
		if (it->first == currentTexture) {
			//If reaches the end of the map, set current texture to first elemetn
			if (std::next(it) == textures.end()) {
				currentTexture = textures.begin()->first;
			}
			else {
				currentTexture = std::next(it)->first;
			}
			break;
		}
		it = std::next(it);
	}
}

void VulkanObject::PrevTexture(){
	//Keep track of the previous texture
	//std::pair<const char*, VulkanMaterial**> prevTexture = {};
	////Loop through to fint current texture
	//for (auto texture : textures) {
	//	prevTexture = texture;
	//	//When current texture found, set it to the previous texture
	//	if (texture.first == currentTexture) {
	//		currentTexture = prevTexture.first;
	//		break;
	//	}
	//}
}

void VulkanObject::SetModelMatrix(const Matrix4& modelMatrix_){
	modelPushConst.normalMatrix = MMath::transpose(MMath::inverse(modelMatrix_));
	modelPushConst.modelMatrix = modelMatrix_;
}

Matrix4 VulkanObject::GetModelMatrix(){
	return modelPushConst.modelMatrix;
}
