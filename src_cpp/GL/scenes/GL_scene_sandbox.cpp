#include <GL/scenes/GL_scene_sandbox.h>
#include <CheerEngine.h>
//#include "../../../Headers/GL/GL_Renderer.h"

namespace GL
{
	namespace SCN
	{
		//Constructors&Destructors
		SandBox::SandBox(CheerEngine* enginePtr) :
			Scene("SandBox", enginePtr)
		{
			//
		}
		SandBox::~SandBox()
		{
			//
		}
		//Main functions
		void SandBox::onUpdate(float deltaTime)
		{
		}
		void SandBox::onRender()
		{
			GL::Renderer::clear(sceneColor);
			/*	//renderer + meshes + sprites
			if (false) {
				if (!m_meshes.empty())
				{
					for (auto mesh : meshes)
					{
						if (mesh != nullptr)
						{
							mesh->setPosition(glm::vec3(position_1[0], position_1[1], position_1[2]));
							mesh->setRotation(glm::vec3(rotation_1[0], rotation_1[1], rotation_1[2]));
							mesh->setScale(glm::vec3(scale_1));
							mesh->draw();
						}
					}
				}
				if (!m_sprites.empty())
				{
					for (auto sprite : sprites)
					{
						if (sprite != nullptr)
						{
							//sprite->setPosition(glm::vec3(position_1[0], position_1[1], position_1[2]));
							//sprite->setRotation(glm::vec3(rotation_1[0], rotation_1[1], rotation_1[2]));
							//sprite->setScale(glm::vec3(scale_1));
							sprite->draw();
						}
					}
				}
				if (m_renderer)
					renderer->draw();
			}*/
		}
		void SandBox::onRenderGUI()
		{
			ImGui::Text("Welcome to the testing Area!");
			ImGui::ColorEdit3("Clear color", sceneColor);

			ImGui::Text("Transformations of the 0-th mesh");
			ImGui::InputFloat3("Position", position_1, -5.0f, 5.0f);
			ImGui::SliderFloat3("Rotation", rotation_1, 0.0f, 360.0f);
			ImGui::DragFloat3("Scale", &scale_1, 0.1f, 10.0f);

			ImGui::Text("Camera configuration");
			//
			ImGui::Checkbox("Camera is perspective:", &isPerspective);
		}

		//Specialized functions
		//
	}
}