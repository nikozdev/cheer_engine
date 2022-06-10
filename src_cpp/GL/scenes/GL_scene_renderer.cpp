#include <GL/scenes/GL_scene_renderer.h>
#include <CheerEngine.h>
#include <Support/ch_LoadMaster.h>

namespace GL
{
	namespace SCN
	{
		//Constructors&Destructors
		RendererScn::RendererScn(CheerEngine* enginePtr) :
			Scene("Renderer", enginePtr)
		{
			m_renderer.setShader(SP::ShaderManager::get().getInstance(SP::CORE_2D));

			SP::LoadMaster::loadIt_bin("\\resources\\data_bin\\vert_rect.bdt",
				(void*)m_renderer.m_vert_rect2d_4, sizeof(m_renderer.m_vert_rect2d_4));
			SP::LoadMaster::loadIt_bin("\\resources\\data_bin\\color_light.bdt",
				(void*)m_renderer.m_lightColor, sizeof(m_renderer.m_lightColor));

			m_tex_data = new unsigned char[]{
				0, 128, 0,	0, 0, 128,
				0, 0, 128,	0, 128, 0
			};
			glGenTextures(1, &m_tex_id);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_tex_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0,
				GL_RGB, GL_UNSIGNED_BYTE, m_tex_data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
			m_enginePtr->getWnd()->getCamera()->Position = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		RendererScn::~RendererScn()
		{
			SP::LoadMaster::saveIt_bin("\\resources\\data_bin\\vert_rect.bdt",
				m_renderer.m_vert_rect2d_4, sizeof(m_renderer.m_vert_rect2d_4));
			SP::LoadMaster::saveIt_bin("\\resources\\data_bin\\color_light.bdt",
				(void*)m_renderer.m_lightColor, sizeof(m_renderer.m_lightColor));

			delete[] m_tex_data;
		}

		//Main functions
		void RendererScn::onUpdate(float deltaTime)
		{
		}
		void RendererScn::onRender()
		{

			m_model = glm::mat4(1.0f);
			m_projView = glm::mat4(1.0f);
			GL_Camera* camPtr = m_enginePtr->getWnd()->getCamera();
			camPtr->setViewMat(camPtr->getLookAtMatrix());
			switch (camPtr->getProjType())
			{
			case (ORTHO):
				camPtr->setProjMat(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 100.0f, -100.0f));
				break;
			case(PERSPECTIVE):
				camPtr->setProjMat(glm::perspective(camPtr->getZoom(), 1.0f,
					//m_enginePtr->getWnd()->SCR_WIDTH / m_enginePtr->getWnd()->SCR_HEIGHT,
					100.0f, -100.0f));
				break;
			}
			m_projView = camPtr->getProjMat() * camPtr->getViewMat();
			GL::Renderer::clear(m_renderer.m_lightColor);
			
			m_renderer.getShader()->use();

			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_tex_id);

			m_renderer.getShader()->setVec3("lightColor", m_renderer.m_lightColor[0], m_renderer.m_lightColor[1], m_renderer.m_lightColor[2]);
			m_renderer.getShader()->setMat4("model", m_model);
			m_renderer.getShader()->setMat4("projView", m_projView);
			m_renderer.getShader()->setInt("material.texture_diffuse", 0);
			m_renderer.drawVAO();
			m_renderer.getShader()->stopUse();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			//m_renderer.draw(m_va, m_ib, m_shader, GL_TRIANGLES);
			//m_renderer.draw();
		}
		void RendererScn::onRenderGUI()
		{
			//if (ImGui::Button("Create New Renderer"))
			ImGui::ColorEdit3("Global Light Color", m_renderer.m_lightColor);
			ImGui::SliderFloat2("Vertex positions: rentangle_0", &m_renderer.m_vert_rect2d_4[0], -1.0f, 1.0f);
			ImGui::SliderFloat2("Vertex positions: rentangle_1", &m_renderer.m_vert_rect2d_4[2], -1.0f, 1.0f);
			ImGui::SliderFloat2("Vertex positions: rentangle_2", &m_renderer.m_vert_rect2d_4[4], -1.0f, 1.0f);
			ImGui::SliderFloat2("Vertex positions: rentangle_3", &m_renderer.m_vert_rect2d_4[6], -1.0f, 1.0f);
			ImGui::InputInt3("Vertexindicies: first triangle", (int*)&m_renderer.m_rect_indices[0]);
			ImGui::InputInt3("VertexIndicies: second triangle", (int*)&m_renderer.m_rect_indices[3]);
			if (ImGui::Button("SwitchDrawType"))
				if (m_drawType == GL_LINES)
					m_drawType = GL_TRIANGLES;
				else
					m_drawType = GL_LINES;
		}

		//Specialized functions
	}
}