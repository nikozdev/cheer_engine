#include <CheerEngine.h>
#include <GL/scenes/GL_scene_manager.h>
#include <support/ch_LoadMaster.h>
#include <GL/GL_GraphicsObj.h>
#include <ESC/Entity.h>

using namespace std::literals::chrono_literals;

//Static variables
GL::GL_Window CheerEngine::m_wnd = GL::GL_Window(800, 600, "CheerEngine_2.0");
lua_State* CheerEngine::currState;

extern std::string dir;

//Functions for Initialize
void CheerEngine::initStates()
{
	addState((dir + "lua_scripts\\states\\core_state.lua").c_str());
}

void CheerEngine::regCppFunctions()
{
	states.top()->regCppFunction("cpp_create_GL_Window", cpp_create_GL_Window);
}

void CheerEngine::regCppTypes()
{
	L::UpValues upVals{ {this, &SP::ShaderManager::get()}};
	states.top()->regType("Shader", upVals, newShader, destroyShader, indexShader, nullptr);
	states.top()->regCppFunction("Shader", "use", useShader);
	states.top()->regCppFunction("Shader", "del", destroyShader);

	states.top()->regCppFunction("Shader", "setBool", setShaderBool);
	states.top()->regCppFunction("Shader", "setInt", setShaderInt);
	states.top()->regCppFunction("Shader", "setFloat", setShaderFloat);

	states.top()->regCppFunction("Shader", "setVec3", setShaderVec3);

	upVals.up_values[1] = &SP::EntityManager::get();
	states.top()->regType("Entity", upVals,
		ESC::Entity::newEntity, ESC::Entity::destroyEntity,
		ESC::Entity::indexEntity, ESC::Entity::newIndexEntity);
	states.top()->regCppFunction("Entity", "say", ESC::Entity::say);
	states.top()->regCppFunction("Entity", "setSprite", ESC::Entity::setSprite);
}

bool CheerEngine::createWindow(int width, int height, const std::string& title)
{
	SP::Timer settingTime = SP::Timer();
	if (m_wnd_support) delete m_wnd_support;
	m_wnd_support = new GL::GL_Window(width, height, title, &CheerEngine::m_wnd);

	return true;
}

//GLFunctions
void CheerEngine::processInput(GL::GL_Window& wnd)
{
	//Movement
	{
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_W) == GLFW_PRESS)
			wnd.getCamera()->keyboardProc(GL::MOVETO_W, deltaTime);
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_S) == GLFW_PRESS)
			wnd.getCamera()->keyboardProc(GL::MOVETO_S, deltaTime);
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_A) == GLFW_PRESS)
		{
			wnd.getCamera()->keyboardProc(GL::LEFT, deltaTime);
		}
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_D) == GLFW_PRESS)
		{
			wnd.getCamera()->keyboardProc(GL::RIGHT, deltaTime);
		}
	}
	//Control
	{
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{//Close the main or secondary window
			if (&m_wnd != &wnd)
			{
				delete &wnd;
				//GL::GL_Window::closeWindow(&wnd);
			}
			else (glfwSetWindowShouldClose(wnd.getWnd(), GL_TRUE));
		}
	}
	//Camera configuration
	{
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_C) == GLFW_PRESS && lastSet > 1.0f)
		{
			lastSet = 0.0f;
			if (wnd.cursorIsDisabled)
			{
				wnd.getCamera()->canRotate = wnd.cursorIsDisabled = false;
				glfwSetInputMode(m_wnd.getWnd(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				wnd.getCamera()->canRotate = wnd.cursorIsDisabled = true;
				glfwSetInputMode(m_wnd.getWnd(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		else if (glfwGetKey(wnd.getWnd(), GLFW_KEY_P) == GLFW_PRESS && lastSet > 1.0f)
		{
			lastSet = 0.0f;
			switch (wnd.getCamera()->getProjType())
			{
			case GL::camProjection::ORTHO:
				wnd.getCamera()->setProjType(GL::camProjection::PERSPECTIVE);
				break;
			case GL::camProjection::PERSPECTIVE:
				wnd.getCamera()->setProjType(GL::camProjection::ORTHO);
				break;
			}
		}
		else if (glfwGetKey(wnd.getWnd(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && (wnd.getCamera()->getSpeed() < GL::MAXSPEED))
			wnd.getCamera()->setSpeed(m_wnd.getCamera()->getSpeed() + 0.001f);
		else if (glfwGetKey(wnd.getWnd(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS && (wnd.getCamera()->getSpeed() > 0.0f))
			wnd.getCamera()->setSpeed(m_wnd.getCamera()->getSpeed() - 0.001f);
	}
	//Call functions from lua script that you changed dynamically
	{
		if (glfwGetKey(wnd.getWnd(), GLFW_KEY_R) == GLFW_PRESS && lastSet > 1.0f)
		{
			lastSet = 0.0f;
			//states.top()->exeScript((dir + "lua_scripts/functions/debug.lua").c_str());
			states.top()->loadFile();
			lua_getglobal(currState, "ReCall");
			if (lua_pcall(currState, 0, 0, 0) != LUA_OK)
			{
				printf("CHEER_ENGINE::PROCESS_INPUT::RECALL: We can't use ReCall function because...\n%s\n",
					lua_tostring(currState, -1));
			}
		}
		else if (glfwGetKey(wnd.getWnd(), GLFW_KEY_G) == GLFW_PRESS && lastSet > 1.0f)
		{
			lastSet = 0.0f;
			states.top()->exeScript((dir + "lua_scripts/functions/debug.lua").c_str());
			lua_getglobal(currState, "Debug");
			if (lua_pcall(currState, 0, 0, 0) != LUA_OK)
			{
				printf("CHEER_ENGINE::PROCESS_INPUT::RECALL: We can't use Debug function because...\n%s\n",
					lua_tostring(currState, -1));
			}
		}
	}
}

//Constructors & Destructors
CheerEngine::CheerEngine()
{
	SP::Timer initTime = SP::Timer();
	//initStates();

	m_scnManager = new GL::SCN::SceneManager(this);
	
	printf("\t----------------\nCHEER_ENGINE::CONSTRUCTOR: CheerEngine is initialized.\n\t----------------\n");
}

CheerEngine::~CheerEngine()
{
	printf("\t----------------\nCHEER_ENGINE::DESTRUCTOR: Destruction...\n\t----------------\n");
	MEM::GlobalAllocator::get().printMemoryUsage();
	SP::Timer destructTime = SP::Timer();

	while (!states.empty())
	{
		delete states.top();
		states.pop();
	}
	currState = nullptr;

	SP::EntityManager::get().destroyAll();
	SP::MaterialManager::get().destroyAll();
	SP::ShaderManager::get().destroyAll();
	SP::TextureManager::get().destroyAll();

	if (m_wnd_support) delete m_wnd_support;
	if (m_scnManager) delete m_scnManager;

	printf("CHEER_ENGINE::DESTRUCTOR: CheerEngine is closed\n");
	MEM::GlobalAllocator::get().printMemoryUsage();
}

//Functions
void CheerEngine::run()
{
	while (!glfwWindowShouldClose(m_wnd.getWnd()))
	{
		updateDt();
		update();
		render();
	}
}

void CheerEngine::update()
{
	//states.top()->update();
	if (m_scnManager)
	{
		m_scnManager->onUpdate(deltaTime);
	}

	processInput(m_wnd);
	if (m_wnd_support) processInput(*m_wnd_support);

	glfwPollEvents();
}

void CheerEngine::render()
{
	//static float sceneColor[3] = { 0.3f, 0.3f, 0.5f };
	//GL::Renderer::clear({ sceneColor });
	//states.top()->render();
	if(m_scnManager)
		m_scnManager->onRender();

	GL::Shader* shaderPtr = NULL;
	glm::mat4 projView = glm::mat4(1.0f);

	//BEGIN RENDERING

	//END RENDERING
	renderGUI();
	glfwSwapBuffers(m_wnd.getWnd());
	if (m_wnd_support)
	{
		GL::GL_Window::setContext(&m_wnd);
		//GL::Renderer::clear(sceneColor);
		glfwSwapBuffers(m_wnd_support->getWnd());
		GL::GL_Window::setContext(&m_wnd);
	}
}

void CheerEngine::renderGUI()
{
	//IMGUI
	if (m_scnManager)
	{
		m_scnManager->onRenderGUI();
	}
	else {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		/*ImGui::Begin("Support Tools");
		ImGui::Text("General configuration");
		ImGui::ColorEdit3("Clear color", sceneColor);

		ImGui::Text("Transformations of the 0-th mesh");
		ImGui::InputFloat3("Position", position_1, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation", rotation_1, 0.0f, 360.0f);
		ImGui::DragFloat3("Scale", &scale_1, 0.1f, 10.0f);

		ImGui::Text("Camera configuration");
		ImGui::SliderFloat3("Camera Position", &m_wnd.getCamera()->Position.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Camera Rotation", &m_wnd.getCamera()->Front.x, 0.0f, 360.0f);
		ImGui::Checkbox("Camera is perspective:", &isPerspective);
		m_wnd.getCamera()->setProjType((GL::camProjection)!isPerspective);*/

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void CheerEngine::addState(const char* lua_state_file)
{//Push new L_state - register everything - load file - initialize L_state
	states.push(new L::L_state(lua_state_file));
	currState = states.top()->getLState();
	//Add in new L_state current engine instance
	lua_pushlightuserdata(currState, this);
	lua_setglobal(currState, CH_ENGINE);
	
	regCppFunctions();
	regCppTypes();

	assert(states.top()->loadFile());
	assert(states.top()->initialize());
}

void CheerEngine::updateDt()
{
	currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	lastSet += deltaTime;
}

// Lua static functions
// GL functions
int CheerEngine::cpp_create_GL_Window(lua_State* L)
{//cpp_createGLWindow("Title", width, height, [additionalWindow])
	std::string title = (std::string)lua_tostring(L, -1);
	int height = (int)lua_tointeger(L, -2);
	int width = (int)lua_tointeger(L, -3);
	//1 = success, 0 = fail
	lua_getglobal(L, CH_ENGINE);
	CheerEngine* chEngine = static_cast<CheerEngine*>(lua_touserdata(L, -1));

	return (int)chEngine->createWindow(width, height, title);
}

// NativeTypes for lua
// Shader functions
int CheerEngine::newShader(lua_State* L)
{//lightShader = Shader:New("vertexPath", "fragmentPath", "geometryPath")
	CheerEngine* chEngine = static_cast<CheerEngine*>(
		lua_touserdata(L, lua_upvalueindex(1)) );
	SP::ShaderManager* shaderManager = static_cast<SP::ShaderManager*>(
		lua_touserdata(L, lua_upvalueindex(2)) );

	const char* vShaderPath = lua_tostring(L, 2);
	const char* fShaderPath = lua_tostring(L, 3);

	void* shaderPtr = (lua_newuserdata(L, sizeof(GL::Shader)));
	new (shaderPtr) GL::Shader(vShaderPath, fShaderPath);
	shaderManager->addInstance(static_cast<GL::Shader*>(shaderPtr));

	luaL_getmetatable(L, "meta_Shader");
	lua_setmetatable(L, -2);
	//Table for user indicies
	lua_newtable(L);
	lua_setuservalue(L, 1);
	return 1;
}

int CheerEngine::destroyShader(lua_State* L)
{
	SP::ShaderManager* shaderManager = static_cast<SP::ShaderManager*>(
		lua_touserdata(L, lua_upvalueindex(2)));

	GL::Shader* shader = static_cast<GL::Shader*>(lua_touserdata(L, -1));

	if (shader)
	{
		shaderManager->removeInstance(shader);
	}

	return 0;
}

int CheerEngine::indexShader(lua_State* L)
{//3 = 2.1 - get property
	GL::Shader* shader = (GL::Shader*)lua_touserdata(L, -2);
	const char* index = lua_tostring(L, -1);
	if (strcmp(index, "id") == 0)
	{
		lua_pushnumber(L, shader->getID());
		return 1;
	}
	else
	{//Get values from script stack
		lua_getuservalue(L, 1);
		if (!lua_isnil(L, -1))
		{//We have user values which are set from the script
			lua_pushvalue(L, 2);
			lua_gettable(L, -2);
		} else {//There is no user values which were set from the script
			lua_getglobal(L, "Shader");
			lua_pushstring(L, index);
			//Get value from table without invoke metamethods
			lua_rawget(L, -2);
		}
		return 1;
	}
}

int CheerEngine::useShader(lua_State* L)
{
	GL::Shader* shader = (GL::Shader*)lua_touserdata(L, -1);
	shader->use();
	return 0;
}

int CheerEngine::setShaderBool(lua_State* L)
{
	GL::Shader* shader = (GL::Shader*)lua_touserdata(L, -3);
	std::string uniformName = (std::string)lua_tostring(L, -2);
	int value = lua_toboolean(L, -1);
	shader->setBool(uniformName, value);
	return 0;
}
int CheerEngine::setShaderInt(lua_State* L)
{
	GL::Shader* shader = (GL::Shader*)lua_touserdata(L, -3);
	std::string uniformName = (std::string)lua_tostring(L, -2);
	int value = lua_tointeger(L, -1);
	shader->setInt(uniformName, value);
	return 0;
}
int CheerEngine::setShaderFloat(lua_State* L)
{
	GL::Shader* shader = (GL::Shader*)lua_touserdata(L, -3);
	std::string uniformName = (std::string)lua_tostring(L, -2);
	float value = lua_tonumber(L, -1);
	shader->setBool(uniformName, value);
	return 0;
}

int CheerEngine::setShaderVec3(lua_State* L)
{//Shader.setVec3("name", vec3); or Shader.setvec3("name", table)
	GL::Shader* shader = nullptr;
	std::string uniformName;
	glm::vec3 vec3(0.0f);

	if (lua_isuserdata(L, -5) && lua_isstring(L, -4))
	{//shader: -5; uniformName: -4, xyz: -3/-2/-1
		shader = static_cast<GL::Shader*>(lua_touserdata(L, -5));
		uniformName = static_cast<std::string>(lua_tostring(L, -4));
		vec3.z = static_cast<float>(lua_tointeger(L, -1));
		vec3.y = static_cast<float>(lua_tointeger(L, -1));
		vec3.x = static_cast<float>(lua_tointeger(L, -1));
	}
	else if (lua_isuserdata(L, -3) && lua_isstring(L, -2) && lua_istable(L, -1))
	{//shader: -3; uniformName: -2; table: -1
		shader = (GL::Shader*)lua_touserdata(L, -3);
		uniformName = static_cast<std::string>(lua_tostring(L, -2));
		L::L_state::tabToVec(L, vec3);
	}
	assert(shader != nullptr);
	shader->setVec3(uniformName,vec3);
	return 0;
}

int CheerEngine::setShaderVec4(lua_State* L)
{//Shader.setVec4("name", x, y, z, w); or Shader.setvec4("name", table)
	GL::Shader* shader = nullptr;
	std::string uniformName;
	glm::vec4 vec4(0.0f);

	if (lua_isuserdata(L, -5) && lua_isstring(L, -4))
	{//shader: -5; uniformName: -4, xyz: -3/-2/-1
		shader = static_cast<GL::Shader*>(lua_touserdata(L, -5));
		uniformName = static_cast<std::string>(lua_tostring(L, -4));
		vec4.w = static_cast<float>(lua_tointeger(L, -1));
		vec4.z = static_cast<float>(lua_tointeger(L, -1));
		vec4.y = static_cast<float>(lua_tointeger(L, -1));
		vec4.x = static_cast<float>(lua_tointeger(L, -1));
	}
	else if (lua_isuserdata(L, -3) && lua_isstring(L, -2) && lua_istable(L, -1))
	{//shader: -3; uniformName: -2; table: -1
		shader = (GL::Shader*)lua_touserdata(L, -3);
		uniformName = static_cast<std::string>(lua_tostring(L, -2));
		L::L_state::tabToVec(L, vec4);
	}
	assert(shader != nullptr);
	shader->setVec4(uniformName, vec4);
	return 0;
}

//DEBUG functions
void CheerEngine::CH_log(const char* reason,const char* location, int line)
{
	printf("CH_LOG::%s at line %d\n%s\n", location, line, reason);
}