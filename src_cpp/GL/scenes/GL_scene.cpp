#include <GL/scenes/GL_scene.h>

namespace GL
{
	namespace SCN
	{
		//Constructors&Destructors
		Scene::Scene(std::string name, CheerEngine* enginePtr)
			: m_name(name), m_enginePtr(enginePtr)
		{
			printf("GL::SCN::SCENE::CONSTRUCTOR: %s_scene has been created.\n", m_name.c_str());
		}
		Scene::~Scene()
		{
			printf("GL::SCN::SCENE::DESTRUCTOR: %s_scene has been destroyed.\n", m_name.c_str());
		}
	}
}

/*Template for any scene
* // header file

#include "../../../Headers/GL/scenes/GL_scene.h"
namespace GL
{
	namespace SCN
	{
		class SceneTemplate : public Scene
		{
		private:
			// Specialized functions
			//
		public:
			// Constructors&Destructors
			SceneTemplate();
			~SceneTemplate();

			// Accessors
			//

			// Main functions
			virtual void onUpdate(float deltaTime) override;
			virtual void onRender() override;
			virtual void onRenderGUI() override;

			// Specialized functions
			// 
		};
	}
}

* source code file
#include "../../../Headers/GL/scenes/GL_scene.h"
namespace GL
{
	namespace SCN
	{
		//Constructors&Destructors
		SceneTemplate::SceneTemplate() :
			Scene("SceneTemplate")
		{
		}
		SceneTemplate::~SceneTemplate()
		{
		}

		//Main functions
		void SceneTemplate::onUpdate(float deltaTime)
		{
		}
		void SceneTemplate::onRender()
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		}
		void SceneTemplate::onRenderGUI()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin(m_name.c_str());

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		//Specialized functions
		//
	}
}
*/