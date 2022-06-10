#include <CheerEngine.h>
#include <ESC_header.hpp>

#include <filesystem>

static bool isFinished = false;
std::string dir;

int main(int numArgs, char* args[])
{
	if (true)
	{
		// Get the path and the name of executable file
		dir = std::filesystem::current_path().string();
		printf("MAIN::LINE_%d: Here is your project directory:\n%s\n\n",
			__LINE__, dir.c_str());
	}
	if (true)
	{
		CheerEngine chEngine = CheerEngine();
		chEngine.run();
		isFinished = true;
	}

	printf("MAIN::END: Program is going out.\n");
	//Here have to be 448 bytes allocated...
	//IDK why but in the beginning there allocated 16 and 432 bytes
	MEM::GlobalAllocator::get().printMemoryUsage();
	return 0;
}