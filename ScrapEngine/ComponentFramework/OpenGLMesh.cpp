#include "OpenGLMesh.h"

using namespace MATH;

OpenGLMesh::~OpenGLMesh(){
	OnDestroy();
}

bool OpenGLMesh::OnCreate(){
    if (isCreated) return isCreated;
    isCreated = true;
    return isCreated;
}

void OpenGLMesh::OnDestroy(){
    if (!isCreated) return;
    
    isCreated = false;
}

void OpenGLMesh::Update(const float deltaTime_)
{
}

void OpenGLMesh::Render() const{
    //glBindVertexArray(vao);
    //glDrawArrays(drawmode, 0, dataLength);
    //glBindVertexArray(0); // Unbind the VAO
}

