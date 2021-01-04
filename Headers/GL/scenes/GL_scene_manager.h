#ifndef GL_SCENE_MENU_H
#define GL_SCENE_MENU_H

#include "GL_scene.h"
#include <vector>

namespace GL
{
	namespace SCN
	{
		class SceneManager : public Scene
		{
		private:
			std::vector<Scene*> m_scenes;
			Scene* m_currScenePtr = nullptr;
			// Specialized functions
		public:
			//Constructors&Destructors
			SceneManager(CheerEngine* enginePtr);
			~SceneManager();

			// Accessors
			Scene* getScenePtr(const std::string& name) const {
				for (auto& scene : m_scenes)
				{
					if (scene->getName() == name)
						return scene;
				}
			}
			Scene* getCurrScene() const { return m_currScenePtr; }
			// Modificators
			void addScene(Scene* newScene) { m_scenes.push_back(newScene); }

			//Main functions
			virtual void onUpdate(float deltaTime) override;
			virtual void onRender() override;
			virtual void onRenderGUI() override;
			
			//Specialized functions
			//void addScene();
			//void switchScene();
		};
	}
}

#endif