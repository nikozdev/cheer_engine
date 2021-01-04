#ifndef GL_SCENE_RENDERER_H
#define GL_SCENE_RENDERER_H

#include "GL_scene.h"
#include <GL/gl_Renderer.h>
#include <GL/GL_types.h>

// Predefenitions
namespace GL
{
	//
}

namespace GL
{
	namespace SCN
	{
		class RendererScn : public Scene
		{
		private:
			Renderer m_renderer;
			VertexArr m_va;
			VertexBuf m_vo;
			IndexBuf m_ib;
			BufferLayout m_layout;

			glm::mat4 m_model;
			glm::mat4 m_projView;

			unsigned int m_tex_id;
			unsigned char* m_tex_data;

			GLenum m_drawType = GL_TRIANGLES;
			// Specialized functions
		public:
			// Constructors&Destructors
			RendererScn(CheerEngine* enginePtr);
			~RendererScn();
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