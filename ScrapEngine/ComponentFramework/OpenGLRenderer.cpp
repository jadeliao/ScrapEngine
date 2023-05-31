#include <glew.h>
#include "OpenGLRenderer.h"
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include "Debug.h"
#include "OpenGLActor.h"
#include "UBO_Padding.h"
#include "UIManager.h"
#include "LineData.h"
#include "ProfilingTool.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

OpenGLRenderer::OpenGLRenderer(): window(nullptr), context(NULL) {
	setRendererType(RendererType::OPENGL);
}

OpenGLRenderer::~OpenGLRenderer(){
	OnDestroy();
}


bool OpenGLRenderer::OnCreate() { 
	context = SDL_GL_CreateContext(window);
	int major, minor;
	getInstalledOpenGLInfo(&major, &minor);
	setAttributes(major, minor);

	/// Fire up the GL Extension Wrangler
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		Debug::FatalError("Glew initialization failed", __FILE__, __LINE__);
		return false;
	}
	glViewport(0, 0, width, height);
	//Bind uniform buffers for camera and light
	bindCameraUbo();
	bindGLightUbo();
	return true;
}


void OpenGLRenderer::OnDestroy() {

	//Clean up UBOs
	glDeleteBuffers(1, &glightUbo.uboID);
	glDeleteBuffers(1, &cameraUbo.uboID);
	//Clean up model
	for (auto mesh_ : assetPool->modelList) {
		Ref<OpenGLMesh> openGLMesh = std::dynamic_pointer_cast<OpenGLMesh>(mesh_.second);
		glDeleteBuffers(1, openGLMesh->getVbo());
		glDeleteVertexArrays(1, openGLMesh->getVao());
	}
	//Clean up texture
	for (auto material_ : assetPool->textureList) {
		Ref<OpenGLMaterial> openGLMaterial = std::dynamic_pointer_cast<OpenGLMaterial>(material_.second);
		glDeleteTextures(1, openGLMaterial->getTextureID());
	}
	//Clean up shader
	for (auto shader_ : assetPool->pipelineList) {
		Ref<OpenGLShader> openGLShader = std::dynamic_pointer_cast<OpenGLShader>(shader_.second);
		unsigned int shaderID = openGLShader->getShaderID();
		unsigned int fragShaderID = openGLShader->getFragShaderID();
		unsigned int vertShaderID = openGLShader->getVertShaderID();
		glDetachShader(shaderID, fragShaderID);
		glDetachShader(shaderID, vertShaderID);
		glDeleteShader(fragShaderID);
		glDeleteShader(vertShaderID);
		glDeleteProgram(shaderID);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}


void OpenGLRenderer::Render() {

	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_CULL_FACE);
	/// Draw your scene here
	for (auto actor : actorList) {
		Ref<Actor> actor_ = actor.second;
		if (!actor_->getVisibility()) continue;
		Ref<OpenGLMaterial> texture = actor_->GetComponent<OpenGLMaterial>();
		Ref<OpenGLShader> shader = actor_->GetComponent<OpenGLShader>();
		Ref<OpenGLMesh> mesh = actor_->GetComponent<OpenGLMesh>();
		glUseProgram(shader->GetProgram());
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, actor_->GetModelMatrix().modelMatrix);
		glUniform1f(shader->GetUniformID("alpha"), actor_->getAlpha());
		GLuint textureID = *texture->getTextureID();
		glBindTexture(GL_TEXTURE_2D, textureID);
		Render(mesh);

	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glBindTexture(GL_TEXTURE_2D, 0);
	//Draw lines
	for (int i = 0; i < lineList.size(); i++) {
		if (lineList[i]->getIsDestroyed()) {
			//Clean up the line
			glDeleteBuffers(1, lineList[i]->getVbo());
			glDeleteVertexArrays(1, lineList[i]->getVao());
			lineList.erase(lineList.begin() + i);
			continue;
		}
		//std::cout << "Line\n";
		//lineList[i]->getStart().print();
		//lineList[i]->getEnd().print();
		drawLineData(lineList[i]);
	}
	glUseProgram(0);


	SDL_GL_SwapWindow(window);
}

bool OpenGLRenderer::AddActor(const char* actorName_, Ref<Actor> actor_){
	if (ContainActor(actorName_)) return false;
	actorList[actorName_] = actor_;
	return true;
}

void OpenGLRenderer::LoadTexture(const char* textureName_, const char* imageName_){
	Ref<OpenGLMaterial> material_ = std::make_shared<OpenGLMaterial>(nullptr, textureName_, imageName_);
	GLuint textureID = *material_->getTextureID();
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	SDL_Surface* textureSurface = IMG_Load(imageName_);
	if (textureSurface == nullptr) {
		return;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);

	SDL_FreeSurface(textureSurface);
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	material_->setTextureID(textureID);
	assetPool->textureList[textureName_] = material_;
}

void OpenGLRenderer::LoadModel(const char* modelName_, const char* modelFile_){
	//Load model and add to asset pool
	Ref<OpenGLMesh> model_ = std::make_shared<OpenGLMesh>(nullptr, modelName_, modelFile_);
	model_->OnCreate();
	loadModel(model_);
	storeMeshData(model_);
	assetPool->modelList[modelName_] = model_;
}

void OpenGLRenderer::LoadShader(const char* shaderName_, const char* vertFile_, const char* fragFile_, const char* geomFile_){
	Ref<OpenGLShader> shader_ = std::make_shared<OpenGLShader>(nullptr, shaderName_, vertFile_, fragFile_, geomFile_);
	bool status;
	status = CompileAttach(shader_);
	if (status == false) return;
	status = Link(shader_);
	if (status == false) return;
	SetUniformLocations(shader_);
	assetPool->pipelineList[shaderName_] = shader_;
}

void OpenGLRenderer::SetCameraUBO(const Matrix4& projection, const Matrix4& view){
	//Set camera ubo and bind it to the buffer
	cameraUbo.cameraData.proj = projection;
	cameraUbo.cameraData.view = view;
	bindCameraUbo();
}

void OpenGLRenderer::SetGLightsUBO(const GlobalLight& glights){
	glightUbo.lightData = glights;
	bindGLightUbo();
}


void OpenGLRenderer::DrawRay(Vec3 start, Vec3 end, float lifeTime) {
	//Use the created shader for drawing ray
	Ref<LineData> lineData_ = std::make_shared<LineData>(start, end, lifeTime);
	storeLineData(*lineData_);
	lineList.push_back(lineData_);
}

void OpenGLRenderer::drawLineData(Ref<LineData> lineData_) {
	Ref<OpenGLShader> shader = std::dynamic_pointer_cast<OpenGLShader>(GetShader("RayShader"));
	glUseProgram(shader->getShaderID());
	glBindVertexArray(*lineData_->getVao());
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0); // Unbind the VAO
}

SDL_Window* OpenGLRenderer::BuildWindow(std::string name_, int width_, int height_){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Debug::FatalError("Failed to initialize SDL", __FILE__, __LINE__);
		return nullptr;
	}

	this->width = width_;
	this->height = height_;
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (window == nullptr) {
		Debug::FatalError("Failed to create a window", __FILE__, __LINE__);
		return nullptr;
	}
	return window;
}

void OpenGLRenderer::getInstalledOpenGLInfo(int* major, int* minor) {
	/// You can to get some info regarding versions and manufacturer
	const GLubyte* version = glGetString(GL_VERSION);
	/// You can also get the version as ints	
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGetIntegerv(GL_MAJOR_VERSION, major);
	glGetIntegerv(GL_MINOR_VERSION, minor);
	Debug::Info("OpenGL version: " + std::string((char*)glGetString(GL_VERSION)), __FILE__, __LINE__);
	Debug::Info("Graphics card vendor " + std::string((char*)vendor), __FILE__, __LINE__);
	Debug::Info("Graphics card name " + std::string((char*)renderer), __FILE__, __LINE__);
	Debug::Info("GLSL Version " + std::string((char*)glslVersion), __FILE__, __LINE__);
	return;
}

void OpenGLRenderer::loadModel(Ref<OpenGLMesh> mesh_){
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	mesh_->clear();

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, mesh_->getFile())) {
		throw std::runtime_error(warn + err);
	}
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vec3 vertex{};
			vertex.x = attrib.vertices[3 * index.vertex_index + 0];
			vertex.y = attrib.vertices[3 * index.vertex_index + 1];
			vertex.z = attrib.vertices[3 * index.vertex_index + 2];

			Vec3 normal{};
			normal.x = attrib.normals[3 * index.normal_index + 0];
			normal.y = attrib.normals[3 * index.normal_index + 1];
			normal.z = attrib.normals[3 * index.normal_index + 2];

			Vec2 uvCoord{};
			uvCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
			uvCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];

			mesh_->addVertex(vertex);
			mesh_->addNormal(normal);
			mesh_->addUvCoord(uvCoord);

		}
	}
}

void OpenGLRenderer::storeMeshData(Ref<OpenGLMesh> mesh_){
	/// These just make the code easier for me to read
	std::vector<Vec3> vertices = mesh_->getVertices();
	std::vector<Vec3> normals = mesh_->getNormals();
	std::vector<Vec2> uvCoords = mesh_->getUvCoords();

#define VERTEX_LENGTH 	(vertices.size() * (sizeof(Vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(Vec3)))
#define TEXCOORD_LENGTH (uvCoords.size() * (sizeof(Vec2)))

	const int verticiesLayoutLocation = 0;
	const int normalsLayoutLocation = 1;
	const int uvCoordsLayoutLocation = 2;

	/// create and bind the VOA
	glGenVertexArrays(1, mesh_->getVao());
	glBindVertexArray(*mesh_->getVao());
	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, mesh_->getVbo());
	glBindBuffer(GL_ARRAY_BUFFER, *mesh_->getVao());
	glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH + TEXCOORD_LENGTH, nullptr, GL_STATIC_DRAW);

	/// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
	glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
	/// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);
	/// assigns the addr of "texCoords" to be "sizeof(points) + sizeof(normals)" offset from the beginning and "sizeof(texCoords)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, TEXCOORD_LENGTH, &uvCoords[0]);

	glEnableVertexAttribArray(verticiesLayoutLocation);
	glVertexAttribPointer(verticiesLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(normalsLayoutLocation);
	glVertexAttribPointer(normalsLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH));
	glEnableVertexAttribArray(uvCoordsLayoutLocation);
	glVertexAttribPointer(uvCoordsLayoutLocation, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH + NORMAL_LENGTH));

	mesh_->setDataLength(vertices.size());

	/// give back the memory used in these vectors. The data is safely stored in the GPU now
	mesh_->clear();

	/// Don't need these defines sticking around anymore
#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
#undef TEXCOORD_LENGTH
}
void OpenGLRenderer::storeLineData(LineData& lineData_) {

	//Create and bind the VAO
	glGenVertexArrays(1, lineData_.getVao());
	glBindVertexArray(*lineData_.getVao());	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, lineData_.getVbo());
	glBindBuffer(GL_ARRAY_BUFFER, *lineData_.getVbo());
	//Size of the data is simply two Vec3 for start and end
	std::vector<Vec3>vertices{ lineData_.getStart(), lineData_.getEnd() };
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) + sizeof(Vec3), &vertices[0], GL_STATIC_DRAW);

	//Location for vecticies is 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

}


char* OpenGLRenderer::ReadTextFile(const char* fileName_){
	char* buffer = nullptr;
	std::ifstream file(fileName_, std::ios::binary);
	if (file.is_open()) {								/// Open the file
		file.seekg(0, std::ios::end);					/// goto the end of the file
		int bufferSize = (int)file.tellg();			/// Get the length of the file
		if (bufferSize == 0) {							/// If zero, bad file
			std::string errorMsg("Can't read shader file: ");
			std::string str2(fileName_);
			errorMsg += str2;
			throw errorMsg;								/// Bail out
		}
		buffer = new char[(int)(bufferSize + 1)];		/// Need to NULL terminate the array
		file.seekg(0, std::ios::beg);
		file.read(buffer, bufferSize);
		buffer[bufferSize] = NULL;						/// Add the NULL
		file.close();
	}
	else {
		std::string errorMsg("Can't open shader file: ");
		errorMsg += fileName_;
		Debug::Error(errorMsg, __FILE__, __LINE__);
	}
	return buffer;
}

bool OpenGLRenderer::CompileAttach(Ref<OpenGLShader> shader_){
	GLint status;
	try {
		const char* vsText = ReadTextFile(shader_->getVsFile());
		const char* fsText = ReadTextFile(shader_->getFsFile());
		if (vsText == nullptr || fsText == nullptr) {
			return false;
		}
		/// GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
		unsigned int vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		/// Check for errors
		if (vertShaderID == 0 || fragShaderID == 0) {
			std::string errorMsg("Can't create a new shader");
			throw errorMsg;
		}
		//Set shader id to shader component
		shader_->setVertShaderID(vertShaderID);
		shader_->setFragShaderID(fragShaderID);

		glShaderSource(vertShaderID, 1, &vsText, 0);
		glShaderSource(fragShaderID, 1, &fsText, 0);

		glCompileShader(vertShaderID);
		/// Check for errors
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status);
		if (status == 0) {
			GLsizei errorLogSize = 0;
			GLsizei titleLength;
			std::string errorLog = shader_->getVsFile();
			errorLog += "\nVERT:\n";
			titleLength = errorLog.length();
			glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
			errorLog.resize(titleLength + errorLogSize);
			glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
			throw errorLog;
		}

		glCompileShader(fragShaderID);
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status);
		if (status == 0) {
			GLsizei errorLogSize = 0;
			GLsizei titleLength;
			std::string errorLog = shader_->getFsFile();
			errorLog += "\nFrag:\n";
			titleLength = errorLog.length();
			glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
			errorLog.resize(titleLength + errorLogSize);
			glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
			throw errorLog;
		}

		unsigned int shaderID = glCreateProgram();
		glAttachShader(shaderID, fragShaderID);
		glAttachShader(shaderID, vertShaderID);
		if (vsText) delete[] vsText;
		if (fsText) delete[] fsText;
		shader_->setShaderID(shaderID);

	}
	catch (std::string error) {
		printf("%s\n", &error[0]);
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

bool OpenGLRenderer::Link(Ref<OpenGLShader> shader_){
	GLint status;
	try {
		unsigned int shaderID = shader_->getShaderID();
		glLinkProgram(shaderID);
		/// Check for errors
		glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
		if (status == 0) {
			GLsizei errorLogSize = 0;
			std::string errorLog;
			glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
			errorLog.resize(errorLogSize);
			glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
			throw errorLog;
		}

	}
	catch (std::string error) {
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

void OpenGLRenderer::SetUniformLocations(Ref<OpenGLShader> shader_){
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char* name;
	int maxUniformListLength;
	unsigned int loc;
	char buffer[512];
	unsigned int shaderID = shader_->getShaderID();

	std::string activeUniformList = "Uniforms:\n";
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	sprintf_s(buffer, 512, "There are %d active Uniforms\n", count);

	/// get the length of the longest named uniform 
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char*)malloc(sizeof(char) * maxUniformListLength);


	for (int i = 0; i < count; ++i) {

		/// Get the name of the ith uniform

		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		std::string uniformName = name;
		shader_->AddUniformID(uniformName, loc);

		sprintf_s(buffer, 512, "\"%s\" loc:%d\n", uniformName.c_str(), loc);
		activeUniformList += buffer;
	}
	Debug::Info(activeUniformList, __FILE__, __LINE__);
	free(name);
}

void OpenGLRenderer::Render(Ref<OpenGLMesh> mesh_) const{
	glBindVertexArray(*mesh_->getVao());
	glDrawArrays(GL_TRIANGLES, 0, mesh_->getDataLength());
	glBindVertexArray(0); // Unbind the VAO

}

void OpenGLRenderer::bindCameraUbo(){
	size_t buffer_size = 2 * UBO_PADDING::MAT4;
	glGenBuffers(1, &cameraUbo.uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraUbo.uboID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	//Bind the camera data to the buffers
	size_t offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), cameraUbo.cameraData.proj);
	offset = UBO_PADDING::MAT4;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Matrix4), cameraUbo.cameraData.view);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, cameraUbo.bindingPoint, cameraUbo.uboID);
}

void OpenGLRenderer::bindGLightUbo(){
	//size_t buffer_size = 2 * UBO_PADDING::VEC4 * LIGHT_NUM; // 2 Vec4[LIGHT_NUM] data
	//glGenBuffers(1, &glightUbo.uboID);
	//glBindBuffer(GL_UNIFORM_BUFFER, glightUbo.uboID);
	//glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	////Bind the light data to the buffers
	//size_t offset = 0;
	//glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4) * LIGHT_NUM, glightUbo.lightData.position);
	//offset = UBO_PADDING::VEC4 * LIGHT_NUM;
	//glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4) * LIGHT_NUM, glightUbo.lightData.diffuse);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, glightUbo.bindingPoint, glightUbo.uboID);

	//Allocate space for a VEC3 (position) and VEC4(colour)
	size_t buffer_size = LIGHT_NUM * UBO_PADDING::VEC4 + LIGHT_NUM * UBO_PADDING::VEC4;
	glGenBuffers(1, &glightUbo.uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, glightUbo.uboID);
	glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	//Start offset from the start
	size_t offset = 0;
	//Put position(Vec4) into subbuffer
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), glightUbo.lightData.position[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset + UBO_PADDING::VEC4, sizeof(Vec4), glightUbo.lightData.position[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset + 2 * UBO_PADDING::VEC4, sizeof(Vec4), glightUbo.lightData.position[2]);
	//Start the offset at the end of position(Vec3)
	offset = LIGHT_NUM * UBO_PADDING::VEC4;
	//Put colour(Vec4) into sub uffer
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), glightUbo.lightData.diffuse[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset + UBO_PADDING::VEC4, sizeof(Vec4), glightUbo.lightData.diffuse[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset + 2 * UBO_PADDING::VEC4, sizeof(Vec4), glightUbo.lightData.diffuse[2]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, glightUbo.bindingPoint, glightUbo.uboID);


}

void OpenGLRenderer::InitImGui(){

	UIManager::getInstance()->setSize(width, height);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); //(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void OpenGLRenderer::setAttributes(int major_, int minor_) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	return;
}


