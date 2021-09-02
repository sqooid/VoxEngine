#include "voxpch.h"
#include "main.h"
#include "Engine/Archsystem.h"

#include "QuadTest.h"

std::string gProgramName = "CubeWorld the II";
int gWindowWidth = 1920;
int gWindowHeight = 1080;

int main()
{
	vox::Archsystem window;
	window.initializeWindow(gProgramName, gWindowWidth, gWindowHeight);
	window.initializeSubsystems();

	window.pushSubsystem<QuadTest>();

	window.startProgramLoop();
	return 0;
}