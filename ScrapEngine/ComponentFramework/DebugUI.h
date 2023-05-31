#ifndef DEBUGUI_H
#define DEBUGUI_H

#include "UserInterface.h"
#include "Scene.h"
#include "ProfilingTool.h"
//
//struct ProfilingResult;

class DebugUI : public UserInterface {

private:
	std::unordered_map<const char*, Ref<ProfilingTool>> currentResult;

	void showProfiling();
	void showDebugMsg();

public:
	DebugUI();
	~DebugUI();
	void Display();
	void Update(Scene* scene_);

};



#endif