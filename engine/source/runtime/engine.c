#include "engine.h"
#include "function/render/window_system.h"

void startEngine()
{
	initialize();
	mainLoop();
	cleanup();
}
