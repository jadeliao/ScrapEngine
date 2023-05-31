#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

//#ifdef _DEBUG
//#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//// allocations to be of _CLIENT_BLOCK type
//#else
//#define DBG_NEW new
//#endif

#include <string>
#include "SceneManager.h"
#include "Debug.h"
#include "MMath.h"
using namespace MATH;
  
int main(int argc, char* args[]) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		bool initialize = false;
		RendererType rendererType;
		std::string name = { "TEngine" };
		Debug::DebugInit(name + "_Log");
		Debug::Info("Starting the GameSceneManager", __FILE__, __LINE__);

		while (!initialize) {
			char rendererChoice[1024] = "";
			std::cout << "Choose renderer type (0)VULKAN (1)OPENGL:";
			std::cin >> rendererChoice;
			//Create Server
			if (strcmp(rendererChoice, "0") == 0) {
				rendererType = RendererType::VULKAN;
				initialize = true;
			}
			//Create Client
			else if (strcmp(rendererChoice, "1") == 0) {
				rendererType = RendererType::OPENGL;
				initialize = true;
			}
			else {
				std::cout << "Invalid renderer type.\n";
			}
		}

		SceneManager* gsm = new SceneManager(rendererType);
		if (gsm->Initialize(name, 1280, 720) == true) {
			gsm->Run();
		}
		delete gsm;
	}
	//_CrtDumpMemoryLeaks();
	exit(0);

}