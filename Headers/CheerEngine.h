#ifndef CHEER_ENGINE_H
#define CHEER_ENGINE_H

#include <GL/GL_Window.h>
#include <GL/GL_Renderer.h>

#include <Lua/L_state.h>

#include <framework.hpp>
#include <math/ch_math.hpp>

/// Name of the engine reference in the lua state
constexpr const char* CH_ENGINE = "CH_ENGINE";

// Predefenitions
namespace ESC
{
	class Entity;
}
namespace GL
{
	class VertexArr;
	class BufferLayout;
	class IndexBuf;
	class VertexBuf;
	namespace SCN
	{
		class SceneManager;
	}
	class Mesh;
	class Sprite;
	class Texture;
	class GL_Window;
}
/// The Engine created as 3Dimentional, but destinated to draw 2D
/// Work had been started 14th August of 2020th.
/// Contains classes of entire project, draws graphics, works with memory and Lua States
class CheerEngine
{
private:
	//Lua variables
	std::stack<L::L_state*> states;
	static lua_State* currState;
	//GL Classes and structs
	static GL::GL_Window m_wnd;
	GL::GL_Window* m_wnd_support = nullptr;

	GL::SCN::SceneManager* m_scnManager = nullptr;

	//TimeProc
	float currentFrame, lastFrame,
		deltaTime, lastSet;
	
	//Initialize lua functions
	void initStates();
	void regCppFunctions();
	void regCppTypes();
	bool createWindow(int width, int height,
		const std::string& title);

	//GL functions
	void processInput(GL::GL_Window& wnd);
public:
	// Constructors & Destructors
	CheerEngine();
	~CheerEngine();

	// Accessors
	L::L_state* getTopState() const { return states.top(); }
	static lua_State* getLState() { return currState; }
	GL::GL_Window* getWnd() const { return &m_wnd; }

	// Base Functions
	void update();
	void updateDt();
	void render();
	void renderGUI();
	void run();

	void addState(const char* lua_state_file);
	
	//Lua Static functions
	static int cpp_create_GL_Window(lua_State* L);

	//Shaders
	static int newShader(lua_State* L);
	static int destroyShader(lua_State* L);
	static int indexShader(lua_State* L);
	static int useShader(lua_State* L);

	static int setShaderBool(lua_State* L);
	static int setShaderInt(lua_State* L);
	static int setShaderFloat(lua_State* L);

	static int setShaderVec3(lua_State* L);
	static int setShaderVec4(lua_State* L);

	//Debug
	static void CH_log(const char* reason, const char* location, int line);
};
#endif
/// --- 07.10.2020 ---
/// Namespaces
/// GL: CWindow, DefCamera, Shader;
/// Also GL: Mesh, Texture, Material, renderer{vao, vbo, ibo, laoyut}(isn't working), LoadMaster
/// L: L_state, ch_lua_managers{LuaManager, EntityManager, ShaderManager}
/// ESC: Entity, Component{GraphicsCmp, NpcCmp, HealthCmp}
/// Support: GlobalMemoryAllocator, ArenaAllocator, Timer