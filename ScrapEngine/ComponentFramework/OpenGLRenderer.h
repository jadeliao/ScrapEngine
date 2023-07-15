#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Renderer.h"
#include "OpenGLMaterial.h"
#include "OpenGLMesh.h"
#include "OpenGLShader.h"
#include "LineData.h"

//class LineData;

struct CameraUBO{
	unsigned int uboID;
	const GLuint bindingPoint = 0;
	CameraMatrices cameraData;
};

struct GLightUBO {
	unsigned int uboID;
	const GLuint bindingPoint = 1;
	GlobalLight lightData;
};

class OpenGLRenderer: public Renderer {
public:

	static Renderer* Initialize() {
		if (!instance_) {
			return new OpenGLRenderer ();
		}
		return nullptr;
	}
	~OpenGLRenderer();
	virtual SDL_Window* BuildWindow(std::string name_, int width_, int height_);
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Render();
	void InitImGui() override;
	bool AddActor(const char*, Ref<Actor> actor_) override;
	void LoadTexture(const char*, const char*) override;
	void LoadModel(const char*, const char*) override;
	void LoadShader(const char* shaderName_, const char*, const char*, const char*) override;
	void SetCameraUBO(const Matrix4& projection, const Matrix4& view);
	void SetGLightsUBO(const GlobalLight& glights);
	void SetGeometryUBO(const GeometryInfo& geometryInfo_){}
	void DrawRay(Vec3 start, Vec3 end, float lifeTime = 1.0f);
	std::vector<Ref<LineData>> getLineList() { return lineList; }

private:
	OpenGLRenderer();

	const char* version = "#version 450";

	/*int width, height;*/
	SDL_Window* window;
	SDL_GLContext context;
	CameraUBO cameraUbo;
	GLightUBO glightUbo;
	std::vector<Ref<LineData>> lineList;

	virtual void initInstance() {}
	void setAttributes(int major_, int minor_);
	void getInstalledOpenGLInfo(int* major, int* minor);

    //Load model helper function
    void loadModel(Ref<OpenGLMesh> mesh_);
	void storeMeshData(Ref<OpenGLMesh> mesh_);
    //Load shader helper function
    char* ReadTextFile(const char* fileName);
    bool CompileAttach(Ref<OpenGLShader> shader_);
    bool Link(Ref<OpenGLShader> shader_);
    void SetUniformLocations(Ref<OpenGLShader> shader_);
	//Load line function
	void storeLineData(LineData& lineData_);
	void drawLineData(Ref<LineData>);

	void Render(Ref<OpenGLMesh> mesh_) const;
	//Bind uniforms
	void bindCameraUbo();
	void bindGLightUbo();
};

#endif
