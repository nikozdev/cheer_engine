#include <GL/GL_TextRenderer.h>

namespace GL
{
	TextRenderer::TextRenderer()
	{
		printf("GL::TEXT_RENDERER_CONSTRUCTOR: TextRenderer is created without arguments.\n");
	}
	TextRenderer::TextRenderer(const char* bitmapPath, int cellSizeX, int cellSizeY, unsigned int charAmount) :
		m_bitmapDirectory(bitmapPath), m_charAmount(charAmount)
	{
		if (charAmount == 0)
			charAmount = cellSizeX * cellSizeY;
		loadFont_def(bitmapPath, cellSizeX, cellSizeY);
		printf("GL::TEXT_RENDERER_CONSTRUCTOR: TextRenderer is created with bitmap.\n");
	}
	TextRenderer::~TextRenderer()
	{
		printf("GL::TEXT_RENDERER_DESTRUCTOR: TextRenderer has been destroyed.\n");
	}

	bool TextRenderer::loadFont_def(const std::string& bitmapPath, int cellSizeX, int cellSizeY)
	{
		std::string format = bitmapPath.substr(bitmapPath.rfind(".", 3));

		if (format == ".dds")
		{
			printf("GL::TEXT_RENDERER::LOAD_FONT_DEFAULT: I can't load dds formats!\n");
			return false;
		}
		else
		{
			return loadFont_stbi(bitmapPath.c_str(), cellSizeX, cellSizeY);
		}
		return true;
	}

	bool TextRenderer::loadFont_stbi(const char* bitmapPath, int cellSizeX, int cellSizeY)
	{
		return false;
	}
}