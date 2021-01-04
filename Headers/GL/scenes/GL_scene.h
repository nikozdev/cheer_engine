#ifndef GL_SCENE_H
#define GL_SCENE_H

#include <vector>
#include <string>

#include <support/imgui/imgui.h>
#include <support/imgui/imgui_impl_opengl3.h>
#include <support/imgui/imgui_impl_glfw.h>
// Predefenitions
class CheerEngine;
namespace GL
{
	//
}

namespace GL
{
	namespace SCN
	{
		class Scene
		{
		protected:
			std::string m_name;
			CheerEngine* m_enginePtr;
		public:
			// Constructors&Destructors
			Scene(std::string name, CheerEngine* enginePtr);
			virtual ~Scene();

			// Accessors
			std::string getName() const { return m_name; }
			void setName(std::string name) { m_name = name; }

			// Main functions
			virtual void onUpdate(float deltaTime) {}
			virtual void onRender() {}
			virtual void onRenderGUI() {}
		};
	}
}


#endif