#ifndef GL_TEXT_RENDERER_H
#define GL_TEXT_RENDERER_H

#include <GL_header.hpp>

#include <string>
#include <vector>

//Predefenitions
namespace GL
{
	struct Letter;
	struct TextRenderer;
}
namespace GL
{
	struct Letter
	{
		char ch = '\0';
		int w = 0, h = 0;
		GLuint textureId;
		Letter() { glGenTextures(1, &textureId); }
		~Letter() { glDeleteTextures(1, &textureId); }
	};

	class TextRenderer
	{
	private:
		std::vector<std::pair<char, Letter>> m_letterDict;
		std::string m_bitmapDirectory = "";
		unsigned int m_charAmount = 0;
	public:
		TextRenderer();
		TextRenderer(const char* bitmapPath, int cellSizeX, int cellSizeY, unsigned int charAmount);
		~TextRenderer();

		std::vector<std::pair<char, Letter>>* getLetters() { return &m_letterDict; }

		static bool loadFont_def(const std::string& bitmapPath, int cellSizeX, int cellSizeY);
		static bool loadFont_stbi(const char* bitmapPath, int cellSizeX, int cellSizeY);
	};
}
#endif