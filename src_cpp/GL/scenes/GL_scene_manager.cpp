#include <GL_scene_header.hpp>
#include <CheerEngine.h>

namespace GL
{
	namespace SCN
	{
		//Constructors&Destructors
		SceneManager::SceneManager(CheerEngine* enginePtr) :
			Scene("Menu", enginePtr)
		{
			//m_scenes.push_back(new SandBox);
			m_scenes.push_back(new RendererScn(enginePtr));
		}
		SceneManager::~SceneManager()
		{
			for (auto& scene : m_scenes)
				if (scene)
					delete scene;
		}

		//Main functions
		void SceneManager::onUpdate(float deltaTime)
		{
			if (m_currScenePtr)
			{
				m_currScenePtr->onUpdate(deltaTime);
				return;
			}
		}
		void SceneManager::onRender()
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.1f, 0.3f, 0.5f, 1.0f);

			if (m_currScenePtr)
			{
				m_currScenePtr->onRender();
			}
		}
		void SceneManager::onRenderGUI()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (m_currScenePtr == nullptr)
			{//Menu is the current scene - check choice of others
				ImGui::Begin(m_name.c_str());
				ImGui::Text("Pick your scene: ");
				for (auto& scene : m_scenes)
				{//Press the button with the name of some scene and set it as current
					if (ImGui::Button(scene->getName().c_str()))
						m_currScenePtr = scene;
				}
			}
			else
			{//Just draw
				ImGui::Begin(m_currScenePtr->getName().c_str());
				m_currScenePtr->onRenderGUI();
				if (ImGui::Button("<- Quit"))
				{//Now the current scene is working - if you press quit button - you will get back 
					m_currScenePtr = nullptr;
				}
			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		
		//Specialized functions
		//
	}
}