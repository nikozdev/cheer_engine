#ifndef GL_SCENE_SANDBOX_H
#define GL_SCENE_SANDBOX_H

#include "GL_scene.h"

namespace GL
{
	namespace SCN
	{
		class SandBox : public Scene
		{
		private:
			//Other stuff
			float sceneColor[3] = { 0.3f, 0.3f, 0.5f };
			float position_1[3] = { 0.0f, 0.0f, 0.0f };
			float rotation_1[3] = { 0.0f, 0.0f, 0.0f };
			float scale_1 = 1.0f;
			bool isPerspective = true;
			// Specialized functions
		public:
			// Constructors&Destructors
			SandBox(CheerEngine* enginePtr);
			~SandBox();

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



#endif